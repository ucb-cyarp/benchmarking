.mode csv
.import report_open_loop_fullness_tracker_raw.csv temp_import_tbl

ATTACH DATABASE 'test.db' as result_db;

CREATE TABLE result_db.results_tbl(
  "Array Length (Blocks)" INT,
  "Block Size (int32_t Elements)" INT,
  "Balancing NOPs" INT,
  "Initial NOPs" INT,
  "Check Period" INT,
  "Steady Clock - Walltime (ms)" REAL,
  "Clock - Cycles/Cycle Time (ms)" REAL,
  "Clock - rdtsc" REAL,
  "Expected Block ID [Core Server]" INT,
  "Start Block ID [Core Server]" INT,
  "End Block ID [Core Server]" INT,
  "Errored [Core Server]" TEXT,
  "Error Source [Core Server]" TEXT,
  "Transaction [Core Server]" INT,
  "Fullness Start [Core Server]" TEXT,
  "Fullness End [Core Server]" TEXT,
  "Interrupts Std Start [Core Server]" TEXT,
  "Interrupts LOC Start [Core Server]" TEXT,
  "Interrupts Other Arch Start [Core Server]" TEXT,
  "Interrupts Std End [Core Server]" TEXT,
  "Interrupts LOC End [Core Server]" TEXT,
  "Interrupts Other Arch End [Core Server]" TEXT,
  "softirq Start [Core Server]" TEXT,
  "softirq End [Core Server]" TEXT,
  "Timing Start [Core Server]" TEXT,
  "Timing End [Core Server]" TEXT,
  "Expected Block ID [Core Client]" INT,
  "Start Block ID [Core Client]" INT,
  "End Block ID [Core Client]" INT,
  "Errored [Core Client]" TEXT,
  "Error Source [Core Client]" TEXT,
  "Transaction [Core Client]" INT,
  "Fullness Start [Core Client]" TEXT,
  "Fullness End [Core Client]" TEXT,
  "Interrupts Std Start [Core Client]" TEXT,
  "Interrupts LOC Start [Core Client]" TEXT,
  "Interrupts Other Arch Start [Core Client]" TEXT,
  "Interrupts Std End [Core Client]" TEXT,
  "Interrupts LOC End [Core Client]" TEXT,
  "Interrupts Other Arch End [Core Client]" TEXT,
  "softirq Start [Core Client]" TEXT,
  "softirq End [Core Client]" TEXT,
  "Timing Start [Core Client]" TEXT,
  "Timing End [Core Client]" TEXT
);

INSERT INTO result_db.results_tbl SELECT * FROM temp_import_tbl;
