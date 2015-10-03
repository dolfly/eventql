/**
 * This file is part of the "libfnord" project
 *   Copyright (c) 2015 Paul Asmuth
 *
 * FnordMetric is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include <zbase/core/RemoteTSDBScan.h>

using namespace stx;

namespace zbase {

RemoteTSDBScan::RemoteTSDBScan(
    RefPtr<csql::SequentialScanNode> stmt,
    const TSDBTableRef& table_ref) :
    stmt_(stmt),
    table_ref_(table_ref),
    rows_scanned_(0) {}

Vector<String> RemoteTSDBScan::columnNames() const {
  return columns_;
}

size_t RemoteTSDBScan::numColumns() const {
  return columns_.size();
}

void RemoteTSDBScan::execute(
    csql::ExecutionContext* context,
    Function<bool (int argc, const csql::SValue* argv)> fn) {
  RAISE(kNotYetImplementedError, "not yet implemented");
}

size_t RemoteTSDBScan::rowsScanned() const {
  return rows_scanned_;
}


} // namespace csql
