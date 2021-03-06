#include "cmdline.h"
// #include "printdata.h"

#include <mysql++.h>

#include <iomanip>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
  // Get database access parameters from command line
  mysqlpp::examples::CommandLine cmdline(argc, argv);
  if (!cmdline) {
    return 1;
  }

  // Connect to the sample database.
  mysqlpp::Connection conn(false);
  if (conn.connect(mysqlpp::examples::db_name, cmdline.server(), cmdline.user(),
                   cmdline.pass())) {
    // Ask for all rows from the sample stock table and display
    // them.  Unlike simple2 example, we retreive each row one at
    // a time instead of storing the entire result set in memory
    // and then iterating over it.
    mysqlpp::Query query = conn.query("select * from stock");
    if (mysqlpp::UseQueryResult res = query.use()) {
      // Display header
      cout.setf(ios::left);
      cout << setw(31) << "Item" 
           << setw(10) << "Num" 
           << setw(10) << "Weight"
           << setw(10) << "Price"
           << "Date" << endl
           << endl;

      // Get each row in result set, and print its contents
      while (mysqlpp::Row row = res.fetch_row()) {
        cout << setw(30) << row["item"] << ' ' 
             << setw(9) << row["num"] << ' '
             << setw(9) << row["weight"] << ' ' 
             << setw(9) << row["price"] << ' ' 
             << setw(9) << row["sdate"] << endl;
      }

      // Check for error: can't distinguish "end of results" and
      // error cases in return from fetch_row() otherwise.
      if (conn.errnum()) {
        cerr << "Error received in fetching a row: " << conn.error() << endl;
        return 1;
      }
      return 0;
    } else {
      cerr << "Failed to get stock item: " << query.error() << endl;
      return 1;
    }
  } else {
    cerr << "DB connection failed: " << conn.error() << endl;
    return 1;
  }
}