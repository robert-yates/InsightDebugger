#include "plugin.h"

#include "fontsettings.h"

WidgetRegisters::WidgetRegisters(QWidget* parent) : QTableWidget(parent) {

    plugin::LogInfo("WidgetRegisters::WidgetRegisters");
    // TODO: implement correctly
    std::vector<QString> registerNames = {"EAX", "EBX", "ECX", "EDX", "ESI", "EDI", "EBP", "ESP", "EIP", "EFL"};

    setColumnCount(2);
    setRowCount(static_cast<int>(registerNames.size()));

    QStringList headers;
    headers << tr("Register") << tr("Value");
    setHorizontalHeaderLabels(headers);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    for (auto& r : registerNames) {
        QTableWidgetItem* registerName = new QTableWidgetItem(r);
        registerName->setFlags(registerName->flags() & ~Qt::ItemIsEditable);
        QTableWidgetItem* registerValue = new QTableWidgetItem("0x00000000");
        setItem(&r - &registerNames[0], 0, registerName);
        setItem(&r - &registerNames[0], 1, registerValue);
    }

    setSelectionBehavior(QAbstractItemView::SelectItems);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    verticalHeader()->setVisible(false);
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    resizeColumnsToContents();
    resizeRowsToContents();
    horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    updateFonts();
}

void WidgetRegisters::updateFonts() {
    m_font = getMonospaceFont(dynamic_cast<QWidget*>(parent()));
    m_font.setKerning(false);
    m_baseline = (int)QFontMetricsF(m_font).ascent();
    m_charWidth = getFontWidthAndAdjustSpacing(m_font);
    m_charHeight = (int)(QFontMetricsF(m_font).height() + getExtraFontSpacing());
    m_charOffset = getFontVerticalOffset();
}

WidgetRegisters::~WidgetRegisters() {}

void WidgetRegisters::updateRegisterValue(int row, uint32_t value) {
    QTableWidgetItem* currentItem = item(row, 1);
    if (currentItem != nullptr)
        currentItem->setText(QString("0x%1").arg(value & 0xFFFFFFFF, 8, 16, QChar('0')));
}
