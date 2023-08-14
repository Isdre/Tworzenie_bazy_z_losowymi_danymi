import json
import psycopg2
import sys

class _ConnectionError(Exception):
    """_ConnectionError"""

class Connection:
    def __init__(self,json_filename : str) -> None:
        with open(json_filename, "r") as access_data_file:
            self.access_data = json.loads(access_data_file.read())
        self.actualDatabase = self.access_data["database"]
        self.connection = psycopg2.connect(database=self.actualDatabase,user=self.access_data["user"],password=self.access_data["password"],host=self.access_data["host"],port=self.access_data["port"])
        self.cursor = self.connection.cursor()

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
            table_parameters = json.loads(access_data_file.read())
        if table_parameters["table_name"] in self.get_tables_list():
            raise _ConnectionError("table already exists")
            return
        variables = ""
        for i in range(table_parameters["table_count"]):
            column = table_parameters[f"column_{i}"]
            variables += " ".join([column["column_name"],column["column_type"]]) + " "
            variables += " ".join(column["constrains"])
            if i != table_parameters["table_count"] - 1: variables += ",\n"
        command = f'''CREATE TABLE {table_parameters["table_name"]}({variables});'''

        self.cursor.execute(command)
        print("Create table successful")

        self.connection.commit()

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

