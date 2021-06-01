#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "mysql.h"

#pragma comment(lib, "libmysql.lib")

const char* host = "localhost";
const char* user = "root";
const char* pw = "mysql";
const char* db = "project";

// year, month에 대한 입력을 숫자로 받아서 이를 string으로 변환하는 함수
void int_to_str(char t[], int n) {
	int cnt = (int)(log10(n));
	while (cnt>=0) {
		t[cnt--] = char(n % 10 + 48);
		n /= 10;
	}
}

int main(void) {

	MYSQL* connection = NULL;
	MYSQL conn;
	MYSQL_RES* sql_result;
	MYSQL_ROW sql_row;

	if (mysql_init(&conn) == NULL)
		printf("mysql_init() error!");

	connection = mysql_real_connect(&conn, host, user, pw, db, 3306, (const char*)NULL, 0);
	if (connection == NULL)
	{
		printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
		return 1;
	}

	else
	{
		printf("Connection Succeed\n");

		if (mysql_select_db(&conn, db))
		{
			printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
			return 1;
		}

		char buffer[400] = { 0 };
		FILE* fp1 = fopen("20150614.txt", "rt");
		while (fgets(buffer,400,fp1)) {
			mysql_query(connection, buffer);
		}
		fclose(fp1);

		int finish = 0, finish1 = 0;
		int state = 0;
		int year, month;
		char type_num1[10];
		char type_num[10];
		while (!finish) {
			finish1 = 0;

			printf("------- SELECT QUERY TYPES -------\n\n");
			printf("\t1. TYPE I\n");
			printf("\t2. TYPE II\n");
			printf("\t3. TYPE III\n");
			printf("\t4. TYPE IV\n");
			printf("\t5. TYPE V\n");
			printf("\t0. QUIT\n");
			printf("Which type of query? ");
			scanf("%s", type_num);
			if (strcmp(type_num, "1") == 0) {
				char code[10];
				printf("\n---- TYPE I ----\n");
				while (!finish1) {
					printf("Input the number of truck : ");
					scanf("%s", code);
					if (!strcmp(code, "1721")) finish1=1;
					else printf("Truck %s is not destroyed.\n\n", code);
				}
				finish1 = 0;
				while (!finish1) {
					printf("\n----- Subtypes int TYPE I -----\n");
					printf("\t1. TYPE I-1.\n");
					printf("\t1. TYPE I-2.\n");
					printf("\t1. TYPE I-3.\n");
					printf("Which type of query? ");
					scanf("%s", type_num1);

					if (strcmp(type_num1, "1") == 0) {
						printf("\n---- TYPE I-1 ----\n");
						printf("** Find all customers who had a package on the truck at the time of the crash.**\n");
						printf("Customer Name : ");
						char q1[200] = "create table crushed as select Package_ID from Transport where transport_type = 'truck' and name = '1721' and delivered = 0;";
						char q2[200] = "create table crushed_customer_id as select distinct B.Customer_ID from Bill B where B.Package_ID in (select c.Package_ID from crushed as c);";
						char q3[200] = "select name from crushed_customer_id natural join Customer; ";
						char q4[100] = "drop table crushed; drop table crushed_customer_id; ";
						mysql_query(connection, q1);
						mysql_query(connection, q2);
						state = mysql_query(connection, q3);
						if (state == 0)
						{
							sql_result = mysql_store_result(connection);
							while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
							{
								printf("%s ", sql_row[0]);
							}
							mysql_free_result(sql_result);
						}
						mysql_query(connection, q4);
						printf("\n");
					}
					else if (strcmp(type_num1, "2") == 0) {
						printf("\n---- TYPE I-2 ----\n");
						printf("** Find all recipients who had a package on that truck at the time of the crash.**\n");
						printf("Recipient Name : ");
						char q1[200] = "create table crushed as select Package_ID from Transport where transport_type = 'truck' and name = '1721' and delivered = 0;";
						char q2[200] = "select name from Recipient R natural join Package P where Package_ID in (select c.Package_ID from crushed as c);";
						char q3[100] = "drop table crushed;";
						mysql_query(connection, q1);
						state = mysql_query(connection, q2);
						if (state == 0)
						{
							sql_result = mysql_store_result(connection);
							while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
							{
								printf("%s ", sql_row[0]);
							}
							mysql_free_result(sql_result);
						}
						mysql_query(connection, q3);
						printf("\n");
					}
					else if (strcmp(type_num1, "3") == 0) {
						printf("\n---- TYPE I-3 ----\n");
						printf("** Find the last successful delivery by that truck prior to the crash **\n");
						printf("Successful Delivery : \n");
						char q1[200] = "create table crushed as select package_id, Datetime from Transport where transport_type='truck' and name = '1721' and delivered = 1;";
						char q2[200] = "create table crushed_package select P1.package_id from Package P1 join crushed C where P1.package_id = c.package_id and Datetime=(select max(C1.Datetime) from crushed C1);";
						char q3[200] = "select P.package_id, package_type, weight, timeliness, matter from Package P join crushed_package c where P.package_id=C.package_id;";
						char q4[100] = "drop table crushed, crushed_package;";
						mysql_query(connection, q1);
						mysql_query(connection, q2);
						state = mysql_query(connection, q3);
						if (state == 0)
						{
							sql_result = mysql_store_result(connection);
							while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
							{
								printf("%s %s %s %s %s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4]);
							}
							mysql_free_result(sql_result);
						}
						mysql_query(connection, q4);
						printf("\n");
					}
					else if (strcmp(type_num1, "0")==0 || strcmp(type_num1, "exit")==0) {
						finish1 = 1;
						printf("\n");
					}
					else {
						printf("Wrong number!\n");
					}
				}
			}
			else if (strcmp(type_num, "2") == 0) {
				printf("\n---- TYPE II ----\n");
				while (!finish1) {
					char year_t1[10] = { 0 };
					char year_t2[10] = { 0 };
					printf("** Find the customer who has shipped the most packages in certain year**\n");
					printf(" Which Year? : ");
					scanf("%d", &year);
					if (year==0) break;
					printf("Customer Name : ");
					char q1[1000] = "create table delivery as select Customer_ID, count(*) num from Bill where date between ";
					char q2[100] = " and ";
					char q3[200] = " group by Customer_ID;";
					char q4[200] = "select name from delivery D1 join Customer C on D1.Customer_ID=C.Customer_ID where num=(select max(D2.num) from delivery D2);";
					char q5[100] = "drop table delivery;";
					int_to_str(year_t1, year * 10000);
					strcat(q1, year_t1);
					strcat(q1, q2);
					int_to_str(year_t2, (year + 1) * 10000);
					strcat(q1, year_t2);
					strcat(q1, q3);
					mysql_query(connection, q1);
					state = mysql_query(connection, q4);
					if (state == 0)
					{
						sql_result = mysql_store_result(connection);
						while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
						{
							printf("%s ", sql_row[0]);
						}
						mysql_free_result(sql_result);
					}
					mysql_query(connection, q5);
					printf("\n\n");
				}
				printf("\n");
			}
			else if (strcmp(type_num, "3") == 0) {
				printf("\n--- TYPE III ----\n");
				while (!finish1) {
					char year_t1[10] = { 0 };
					char year_t2[10] = { 0 };
					printf("** Find the customer who has spent the most money on shipping in the past certain year **\n");
					printf(" Which Year? : ");
					scanf("%d", &year);
					if (year == 0) break;
					printf("Customer Name : ");
					char q1[1000] = "create table delivery as select Customer_ID, sum(amount) pay from Bill where date between ";
					char q2[100] = " and ";
					char q3[200] = " group by Customer_ID;";
					char q4[200] = "select name from delivery D1 join Customer C on D1.Customer_ID=C.Customer_ID where pay=(select max(D2.pay) from delivery D2);";
					char q5[100] = "drop table delivery;";
					int_to_str(year_t1, year * 10000);
					strcat(q1, year_t1);
					strcat(q1, q2);
					int_to_str(year_t2, (year + 1) * 10000);
					strcat(q1, year_t2);
					strcat(q1, q3);
					mysql_query(connection, q1);
					state = mysql_query(connection, q4);
					if (state == 0)
					{
						sql_result = mysql_store_result(connection);
						while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
						{
							printf("%s ", sql_row[0]);
						}
						mysql_free_result(sql_result);
					}
					mysql_query(connection, q5);
					printf("\n");
				}
				printf("\n");
			}
			else if (strcmp(type_num, "4") == 0) {
				printf("\n--- TYPE IV ----\n");
				printf("** Find those packages that were not delivered within the promised time.**\n");
				char q1[100] = "select Package_ID from Time where punctual='False'";
				state = mysql_query(connection, q1);
				if (state == 0)
				{
					sql_result = mysql_store_result(connection);
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						printf("%s ", sql_row[0]);
					}
					mysql_free_result(sql_result);
					printf("\n\n");
				}
			}
			else if (strcmp(type_num, "5") == 0) {
				while (!finish1) {
					char name[10] = { 0 };
					char fname[30] = "bill_";
					char month_t1[10] = { 0 };
					char month_t2[10] = { 0 };

					printf("** Generate the bill for each customer for the past certain month.**\n");
					printf("Customer Name : ");
					scanf("%s", name);
					if (strcmp(name, "0") == 0) break;
					printf(" Which Month(YYYY-MM)? : ");
					scanf("%d-%d", &year, &month);
					if (year == 0 || month == 0) break;
					strcat(fname, name);
					strcat(fname, ".txt");
					FILE* fp = fopen(fname, "wt");
					fprintf(fp, "Bill %s %d %d\n", name, year, month);

					printf("Generating Bill..\n");
					char q1[300] = "create table last_month_bill as select C.customer_id, C.address, B.amount, B.package_id, B.charged_type from Bill B join Customer C where B.customer_id=C.customer_id and C.name='";
					char q2[100] = "' and B.date between ";
					char q3[200] = " and ";
					char q4[200] = "select customer_id, address, SUM(amount) from last_month_bill;";
					char q5[200] = "select package_id, name, amount, package_type, charged_type, timeliness from last_month_bill B1 natural join Package;";
					char q6[100] = "drop table last_month_bill;";
					int_to_str(month_t1, year * 10000 + month * 100);
					int_to_str(month_t2, year * 10000 + (month + 1) * 100);
					strcat(q1, name);
					strcat(q1, q2);
					strcat(q1, month_t1);
					strcat(q1, q3);
					strcat(q1, month_t2);
					strcat(q1, ";");
					mysql_query(connection, q1);

					state = mysql_query(connection, q4);
					fprintf(fp, "Customer  Address  Amount\n");
					if (state == 0)
					{
						sql_result = mysql_store_result(connection);
						while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
						{
							fprintf(fp, "%s %s %s\n", sql_row[0], sql_row[1], sql_row[2]);
						}
						mysql_free_result(sql_result);
					}
					fprintf(fp, "------------------------------\nItemized Billing List\n");
					fprintf(fp, "====================================================================================================\n");
					fprintf(fp, "| Package Number | Item | Amount | Serivce Type | Payment Type | Timeliness of Delivery|\n");
					fprintf(fp, "====================================================================================================\n");
					state = mysql_query(connection, q5);
					if (state == 0)
					{
						sql_result = mysql_store_result(connection);
						while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
						{
							fprintf(fp, "%s | %s | %s | %s | %s | %s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4], sql_row[5]);
						}
						mysql_free_result(sql_result);
					}
					fprintf(fp, "----------------------------------------------------------------------------------------------------");
					mysql_query(connection, q6);
					fclose(fp);
					printf("Generation Completed\n\n");
				}
			}
			else if (strcmp(type_num, "exit") == 0 || strcmp(type_num, "0")==0) {
				finish = 1;
			}
			else {
				printf("Wrong number!\n");
			}
		}
		char query[100] = "drop table Customer, Package, Transport, Time, Recipient, Place, Bill";
		mysql_query(connection, query);
		mysql_close(connection);
	}

	return 0;
}