#ifndef _SQLUTILS_H_
#define _SQLUTILS_H_

#include "sqlite3/include/sqlite3.h"
#include "cocos2d.h"
#include "extensions/cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class SqlUtils
{
public:
	/* Doc hoac tao moi 1 file sql database
	@name std::string ten cua database file hoac ten cua db
	Return sqlite3 : Tra ve doi tuong sqlite3
	*/
	static sqlite3* openData(std::string path);

	/* Tao bang moi trong db
	@db sqlite3: doi tuong sqlite3 dang su dung
	@tableName string: ten cua bang muon them vao
	@primaryKeyField string: ten cua truong duoc set la primaryKey
	@primaryType string: kieu cua truong primaryKey
	@fields vector<string>: Mang ten va kieu cua cac truong khac.VD: {"field1 text" , "field2 int" , "field3 varchar(255) , ..."} 
	Result int: Gia tri int cua code nhan duoc trong truy van sqlite3_exec
	*/
	static int createTable(sqlite3* db, std::string tableName, std::string primaryKeyField, std::string primaryKeyType, std::vector<std::string> fields);

	/*Insert gia tri cho bang vua tao
	 @db sqlite3: doi tuong sqlite3 dang su dung
	 @tableName string: ten cua bang muon insert value vao
	 @fields vector<string>: mang cac truong can them gia tri.VD: {"field1" , "field2" , "filed3" , ...}
	 @values vector<string>: mang cac gia tri them vao cac truong tuong ung. VD: {"valueField1" , "valueField2" , "valueField3" , ...}
	 Result int: Gia tri int cua code nhan duoc trong truy van sqlite3_exec
	*/
	static int insertRow(sqlite3* db , std::string tableName , std::vector<std::string> fields ,std::vector<std::string> values);

	/*Xoa 1 hang tu bang
	 @db sqlite3: doi tuong sqlite3 dang su dung
	 @tableName string: ten cua bang muon insert value vao
	 @fieldsName string: ten truong muon xoa
	 @value string: gia tri cua truong can xoa
	 @stringFlg bool = false : set gia tri true neu data cua truong do la text,string,vachar
	 Result int: Gia tri int cua code nhan duoc trong truy van sqlite3_exec
	*/
	static int deleteRow(sqlite3* db , std::string tableName , std::string fieldName ,std::string value , bool stringFlg=false);

	/*Thuc thi lenh query
	@db sqlite3: doi tuong sqlite3 dang su dung
	@query string: lenh truy van
	ResultÅ@vector<vector<string>>: Tra ve true neu query duoc thuc thi va false neu khong duoc thuc thi
	*/
	static vector<vector<string>> runQuery(sqlite3* db, std::string query);

	/*Thuc thi lenh xoa truncate(khong ghi nhan lai transaction log)*/
	static int truncateTable(sqlite3* db, std::string tableName);
	/*Thuc thi lenh xoa delete(khong lam thay doi cau truc bang)*/
	static int dropTable(sqlite3* db, std::string tableName);
	/*Dong CSDL sau khi su dung xong*/
	static void closeSQL(sqlite3* db);


private:

};

#endif // _SQLUTILS_H_