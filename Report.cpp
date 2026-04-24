#include "Report.h"
#include "PriceUtils.h"

void generateReport(const Warehouse& wh, std::ostream& out) {
    out << "=== Inventory Report ===" << std::endl;
    out << wh.getName() << std::endl;
    out << "Items: " << wh.itemCount() << std::endl;
    for (int i = 0; i < wh.itemCount(); i++) {
        out << "  " << wh.getItem(i) << std::endl;
    }
    double total = wh.totalValue();
    double taxed = applyTax(total, 8.5);
    out << "Subtotal: " << formatPrice(total) << std::endl;
    out << "Total (with 8.5% tax): " << formatPrice(taxed) << std::endl;
}

std::string generateSummary(const Warehouse& wh) {
    return wh.getName() + ": " + std::to_string(wh.itemCount()) + " items, "
           + formatPrice(wh.totalValue());
}
