/**
 * This file is part of the "tsdb" project
 *   Copyright (c) 2015 Paul Asmuth, FnordCorp B.V.
 *
 * FnordMetric is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include <stx/fnv.h>
#include <stx/io/fileutil.h>
#include <sstable/sstablereader.h>
#include <tsdb/PartitionReader.h>

using namespace stx;

namespace tsdb {

PartitionReader::PartitionReader(
    RefPtr<PartitionSnapshot> head) :
    snap_(head) {}

void PartitionReader::fetchRecords(
    size_t offset,
    size_t limit,
    Function<void (
        const SHA1Hash& record_id,
        const void* record_data,
        size_t record_size)> fn) {
  int64_t remaining = snap_->nrecs;
  const auto& files = snap_->state.sstable_files();
  for (const auto& f : files) {
    auto fpath = FileUtil::joinPaths(snap_->base_path, f);
    sstable::SSTableReader reader(fpath);

    auto nrows = reader.countRows();
    if (offset > nrows) {
      offset -= nrows;
      remaining -= nrows;
      continue;
    }

    if (remaining <= 0) {
      return;
    }

    auto cursor = reader.getCursor();
    while (cursor->valid()) {
      void* key;
      size_t key_size;
      cursor->getKey(&key, &key_size);
      if (key_size != SHA1Hash::kSize) {
        RAISE(kRuntimeError, "invalid row");
      }

      void* data;
      size_t data_size;
      cursor->getData(&data, &data_size);

      if (offset > 0) {
        --offset;
      } else {
        fn(SHA1Hash(key, key_size), data, data_size);

        if (limit != size_t(-1) && --limit == 0) {
          return;
        }
      }

      if (--remaining <= 0) {
        return;
      }

      if (!cursor->next()) {
        break;
      }
    }
  }
}

void PartitionReader::fetchRecords(Function<void (const Buffer& record)> fn) {
  fetchRecordsWithSampling(
      0,
      0,
      fn);
}

void PartitionReader::fetchRecordsWithSampling(
    size_t sample_modulo,
    size_t sample_index,
    Function<void (const Buffer& record)> fn) {
  FNV<uint64_t> fnv;

  auto nrows = snap_->nrecs;
  const auto& files = snap_->state.sstable_files();
  for (const auto& f : files) {
    auto fpath = FileUtil::joinPaths(snap_->base_path, f);
    sstable::SSTableReader reader(fpath);
    auto cursor = reader.getCursor();

    while (cursor->valid()) {
      void* key;
      size_t key_size;
      cursor->getKey((void**) &key, &key_size);
      if (key_size != SHA1Hash::kSize) {
        RAISE(kRuntimeError, "invalid row");
      }

      if (sample_modulo == 0 ||
          (fnv.hash(key, key_size) % sample_modulo == sample_index)) {
        void* data;
        size_t data_size;
        cursor->getData(&data, &data_size);

        fn(Buffer(data, data_size));
      }

      if (--nrows == 0) {
        return;
      }

      if (!cursor->next()) {
        break;
      }
    }
  }
}

Option<RefPtr<VFSFile>> PartitionReader::fetchSecondaryIndex(
    const String& index) const {
  return None<RefPtr<VFSFile>>();
}

} // namespace tdsb

