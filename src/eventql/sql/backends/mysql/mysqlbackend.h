/**
 * Copyright (c) 2016 DeepCortex GmbH <legal@eventql.io>
 * Authors:
 *   - Paul Asmuth <paul@eventql.io>
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Affero General Public License ("the license") as
 * published by the Free Software Foundation, either version 3 of the License,
 * or any later version.
 *
 * In accordance with Section 7(e) of the license, the licensing of the Program
 * under the license does not imply a trademark license. Therefore any rights,
 * title and interest in our trademarks remain entirely with us.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the license for more details.
 *
 * You can be released from the requirements of the license by purchasing a
 * commercial license. Buying such a license is mandatory as soon as you develop
 * commercial activities involving this program without disclosing the source
 * code of your own applications
 */
#ifndef _FNORDMETRIC_MYSQLBACKEND_H
#define _FNORDMETRIC_MYSQLBACKEND_H
#include <eventql/sql/backends/backend.h>
#include <eventql/sql/backends/mysql/mysqlconnection.h>
#include <memory>
#include <mutex>
#include <vector>

namespace csql {
namespace mysql_backend {

class MySQLBackend : public csql::Backend {
public:

  static MySQLBackend* singleton();

  MySQLBackend();

  bool openTables(
      const std::vector<std::string>& table_names,
      const URI& source_uri,
      std::vector<std::unique_ptr<TableRef>>* target) override;

protected:
  std::vector<std::shared_ptr<MySQLConnection>> connections_;
  std::mutex connections_mutex_;
};

}
}
#endif
