#ifndef REPORT_H
#define REPORT_H

#include <iostream>
#include <string>
#include "Warehouse.h"

void generateReport(const Warehouse& wh, std::ostream& out);
std::string generateSummary(const Warehouse& wh);

#endif
