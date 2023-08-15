import json
import string
import psycopg2
import sys
import pandas as pd
import numpy as np
import random
import datetime
import time
from sqlalchemy import create_engine

def random_date(start, end, time_format = "%d/%m/%Y"):
    stime = time.mktime(time.strptime(start, time_format))
    etime = time.mktime(time.strptime(end, time_format))
    prop = random.random()
    ptime = stime + prop * (etime - stime)

    return time.strftime(time_format, time.localtime(ptime))

class _ConnectionError(Exception):
    """_ConnectionError"""

class Connection:
    def __init__(self,json_filename : str) -> None:
        with open(json_filename, "r") as access_data_file:
            self.access_data = json.loads(access_data_file.read())
        self.actualDatabase = self.access_data["database"]
        self.connection = psycopg2.connect(database=self.actualDatabase,user=self.access_data["user"],password=self.access_data["password"],host=self.access_data["host"],port=self.access_data["port"])
        self.cursor = self.connection.cursor()
        self.table_parameters = None

    def __del__(self) -> None:
        self.connection.close()

    def get_databases_list(self) -> list[str]:
        self.cursor.execute('''SELECT datname FROM pg_database;''')
        result = [i[0] for i in self.cursor.fetchall()]
        return result

    def get_tables_list(self) -> list[str]:
        self.cursor.execute('''SELECT * FROM pg_catalog.pg_tables WHERE schemaname != 'pg_catalog' AND schemaname != 'information_schema';''')
        result = [i[1] for i in self.cursor.fetchall()]
        return result

    def create_table_with_data(self,json_filename : str, howMany : int = 100) -> None:
        with open(json_filename, "r") as access_data_file:
            self.table_parameters = json.loads(access_data_file.read())
        if self.table_parameters["table_name"] in self.get_tables_list():
            raise _ConnectionError("table already exists")
            return
        variables = ""
        columns = []
        for i in range(self.table_parameters["table_count"]):
            column = self.table_parameters[f"column_{i}"]
            columns.append(column)
            variables += " ".join([column["column_name"],column["column_type"]]) + " "
            variables += " ".join(column["constrains"])
            if i != self.table_parameters["table_count"] - 1: variables += ",\n"
        command = f'''CREATE TABLE {self.table_parameters["table_name"]}({variables});'''

        self.cursor.execute(command)
        print("Create table successful")

        self.connection.commit()
        self.generate_data(columns, howMany)

    def generate_data(self, columns : list, count : int):
        data = pd.DataFrame([])
        for col in columns:
            if col["column_type"] == "SERIAL":
                #data[col["column_name"]] = range(1,count+1)
                continue
            elif col["column_type"] == "BOOLEAN":
                if "UNIQUE" in col["constrains"] and count > 2: raise _ConnectionError("Unique boolean and count > 2")
                data[col["column_name"]] = [bool(random.choice([0,1])) for i in range(count)]
            elif col["column_type"] == "INTEGER":
                dat = []
                if "UNIQUE" in col["constrains"]:
                    dat = set(dat)
                    while len(dat) < count:
                        dat.add(random.randint())
                else:
                    for i in range(count): dat.append(random.randint())
                data[col["column_name"]] = list(dat)
            elif col["column_type"] == "REAL":
                dat = []
                if "UNIQUE" in col["constrains"]:
                    dat = set(dat)
                    while len(dat) < count:
                        dat.add(random.uniform())
                else:
                    for i in range(count): dat.append(random.uniform())
                data[col["column_name"]] = list(dat)
            elif col["column_type"] == "DATE":
                dat = []
                now = datetime.now()
                if "UNIQUE" in col["constrains"]:
                    dat = set(dat)
                    while len(dat) < count:
                        dat.add(random_date("1/1/1969",now.strftime("%d/%m/%Y")))
                else:
                    for i in range(count): dat.append(random_date("1/1/1969",now.strftime("%d/%m/%Y")))
                data[col["column_name"]] = list(dat)
            else:
                dat = []
                if "UNIQUE" in col["constrains"]:
                    dat = set(dat)
                    while len(dat) < count:
                        dat.add("".join([string.ascii_letters for i in range(20)]))
                else:
                    for i in range(count): dat.append("".join([string.ascii_letters for i in range(20)]))
                data[col["column_name"]] = list(dat)

        engine = create_engine(f'postgresql+psycopg2://{self.access_data["user"]}:{self.access_data["password"]}@{self.access_data["host"]}:{self.access_data["5432"]}/{self.access_data["database"]}')

        data.to_sql(self.table_parameters["table_name"], engine, if_exists='append',index=False)


if __name__ == "__main__":
    log = sys.argv[1]
    mod = sys.argv[2]

    #print([log,mod])

    connect = Connection(log)
    if mod == "databases":
        print(" ".join(connect.get_databases_list()))
    elif mod == "tables":
        print(" ".join(connect.get_tables_list()))
    else:
        connect.create_table_with_data(mod)

