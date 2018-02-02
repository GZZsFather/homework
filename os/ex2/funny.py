import pymysql

config = {
          'host':'127.0.0.1',
          'port':3306,
          'user':'root',
          'password':'gzzsb',
          'db':'',
          'charset':'utf8mb4',
          }

connection = pymysql.connect(**config)

sql = '''
create database test；
use test;

create table test1(
    int_column int(10) not null,
    char_column varchar(10) not null
)charset = utf8;
'''

with connection.cursor() as cursor:
        cursor.execute(sql)
        for i in range(1,10):#这里改个数字就行了
            sql = 'insert into test values('+str(i)+','+str(i)+');'
            print(sql)
            cursor.execute(sql)

connection.commit()
connection.close()
