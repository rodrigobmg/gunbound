/* 
Page tham khao code
https://wonkathi515.wordpress.com/2014/09/07/su-dung-sqlite-trong-cocos2d-x/
Xay dung lop thao tac voi du lieu sqlite
*/

#include "Database/SqlUtils.h"

/* Tao csdl moi trong sqlite */
sqlite3* SqlUtils::openData(std::string dbName)
{
	sqlite3* pDB = NULL;
	char* errorMessager = NULL;

	std::string sqlString;
	int result;

	string dbPath = FileUtils::getInstance()->getWritablePath() + dbName;
	log("PATH: %s", dbPath.c_str());

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

	dbPath = FileUtils::getInstance()->getWritablePath();
	dbPath += dbName;

	log("Path Android: %s", dbPath.c_str());

	FILE* file = fopen(dbPath.c_str(), "r");
	if (file != nullptr)
	{
		ssize_t size;
		const char* data = (char*)FileUtils::getInstance()->getFileData(dbPath.c_str(), "rb", &size);
		file = fopen(dbPath.c_str(), "wb");
		fwrite(data, size, 1, file);
		CC_SAFE_DELETE(data);
	}
	fclose(file);


#endif

	result = sqlite3_open(dbPath.c_str(), &pDB);
	/* //or
	result = sqlite3_open_v2(dbPath.c_str() , &pDB , SQLITE_OPEN_READWRITE , NULL);
	*/

	if (result != SQLITE_OK)
	{
		return NULL;
	}

	return pDB;
	
}

/* Tao 1 bang csdl moi */
int SqlUtils::createTable(sqlite3* db, std::string tableName, std::string primaryKeyField, std::string primaryKeyType, std::vector<std::string> fields)
{
	std::string  query = "create table ";
	query.append(tableName).append(" (").append(primaryKeyField).append(" ").append(primaryKeyType).append(" primary key autoincrement, ");
	for (std::vector<std::string>::iterator i = fields.begin(); i != fields.end(); ++i)
	{
		query.append(*i);
		if ((i+ 1) != fields.end())
		{
			query.append(", ");
		}
	}
	query.append(")");
	log("Query: %s ", query.c_str());
	sqlite3_stmt* statement;

	int result = sqlite3_prepare_v2(db, query.c_str(), -1, &statement, NULL);
	if (result == SQLITE_OK)
	{
		int res = sqlite3_step(statement);
		result = res;
		sqlite3_finalize(statement);
		return result;
	}

	return result;

}
/* insert them 1 hang vao bang */
int SqlUtils::insertRow(sqlite3* db, std::string tableName, std::vector<std::string> fields, std::vector<std::string> values)
{
	std::string queryStr = "";
	queryStr.append("insert into ").append(tableName).append("(");
	for (vector<string>::iterator i = fields.begin(); i != fields.end(); ++i)
	{
		queryStr.append(*i);
		if (i + 1 != fields.end())
		{
			queryStr.append(", ");
		}
	}
	queryStr.append(") values('");
	for (vector<string>::iterator i = values.begin(); i != values.end(); ++i)
	{
		queryStr.append(*i);
		if (i + 1 != values.end())
		{
			queryStr.append("', '");
		}
	}
	queryStr.append("')");
	char *str = &queryStr[0];
	cocos2d::log("QUERY %s", queryStr.c_str());
	sqlite3_stmt *statement;
	int result = sqlite3_prepare_v2(db, queryStr.c_str(), -1, &statement, NULL);
	if (result == SQLITE_OK)
	{
		int res = sqlite3_step(statement);
		result = res;
		sqlite3_finalize(statement);
		return res;
	}

	return result;
}

/* Xoa 1 truong khoi bang csdl */
int SqlUtils::deleteRow(sqlite3*db, string table, string fieldName, string value, bool stringFlg)
{
	string query;
	if (stringFlg = false) {
		query = "delete from " + table + " where " + fieldName + " = " + value;
	}
	else {
		query = "delete from " + table + " where " + fieldName + " = '" + value + "'";
	}

	sqlite3_stmt *statement;
	cocos2d::log("QUERY %s", query.c_str());
	int result = sqlite3_prepare(db, query.c_str(), -1, &statement, 0);
	if (result == SQLITE_OK)
	{
		int res = sqlite3_step(statement);
		result = res;
		sqlite3_finalize(statement);
		return res;
	}
	return result;
}

/* Run query truy van csdl */
vector<vector<string>> SqlUtils::runQuery(sqlite3*db, string query)
{
	sqlite3_stmt *statement;
	vector<vector<string>> str;
	if (sqlite3_prepare_v2(db, query.c_str(), -1, &statement, 0) == SQLITE_OK)
	{
		//int res = sqlite3_step(statement);
		int columNum = sqlite3_column_count(statement);
		log("%d", columNum);
		for (;;) {
			int rc = sqlite3_step(statement);
			if (rc == SQLITE_DONE)
				break;
			if (rc != SQLITE_ROW) {
				printf("error: %s!\n", sqlite3_errmsg(db));
				break;
			}
			vector<string> temp;
			for (int i = 0; i < columNum; i++)
			{
				char *text0 = (char *)sqlite3_column_text(statement, i);
				temp.push_back(text0);
			}
			str.push_back(temp);
			temp.clear();
		}
		//result = res;
		sqlite3_finalize(statement);
		return str;
	}
	else {
		cocos2d::log("some error");
	}
	return str;
}

int SqlUtils::truncateTable(sqlite3* db, string tableName)
{
	sqlite3_stmt *statement;
	string query = "TRUNCATE TABLE " + tableName;
	int result = sqlite3_prepare(db, query.c_str(), -1, &statement, 0);
	if (result == SQLITE_OK)
	{
		int res = sqlite3_step(statement);
		result = res;
		sqlite3_finalize(statement);
		return res;
	}
	return result;
}

int SqlUtils::dropTable(sqlite3* db, string tableName)
{
	sqlite3_stmt *statement;
	string query = "DROP TABLE " + tableName;
	int result = sqlite3_prepare(db, query.c_str(), -1, &statement, 0);
	if (result == SQLITE_OK)
	{
		int res = sqlite3_step(statement);
		result = res;
		sqlite3_finalize(statement);
		return res;
	}
	return result;
}

void SqlUtils::closeSQL(sqlite3* db)
{
	sqlite3_close(db);
}