#ifndef GUI_TRANSACTIONS_WIDGET_H
#define GUI_TRANSACTIONS_WIDGET_H

////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#include "db_transactionsdata.h"
#include "db_customersdata.h"
#include "db_storagesdata.h"
#include "gui_newdealdlg.h"

#include "ui_gorcarqner.h"

// Qt includes
#include <QWidget>
#include <QStringListModel>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace fm {
////////////////////////////////////////////////////////////////////////////////
namespace gui {
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
// class CTransactionsWidget
//
class CTransactionsWidget : public QWidget
{
	Q_OBJECT
public:// Constructors
	CTransactionsWidget(QWidget* pwParent = nullptr, std::shared_ptr<db::CDBManager> pDBManager = nullptr);
	~CTransactionsWidget() = default;

public:// Interface Methodes
	void SetDBManager(std::shared_ptr<db::CDBManager> pDBManager);
	inline std::shared_ptr<QSqlTableModel> GetTableModel();

protected:// Helper Methodes
	inline void UpdateData(bool bFull = false);


protected slots:// Slots
	void onChangeData()
	{
		UpdateData(true);
	}
	void onActivatedCustomer(QModelIndex const& modelIndex)
	{
		QString strSelectCustomerName = m_pStringListModel->data(modelIndex, 0).toString();
		if (strSelectCustomerName == m_strCurrentCustomerName)
			return;
		
		m_strCurrentCustomerName = strSelectCustomerName;
	
		UpdateData();
	}
private:// Members
	std::shared_ptr<db::CTransactionsData> m_pTransactionsData;
	std::shared_ptr<db::CCustomersData> m_pCustomersData;
	std::shared_ptr<db::CStoragesData> m_pStoragesData;
	std::shared_ptr<QStringListModel> m_pStringListModel;
	QString m_strCurrentCustomerName;

	Ui::widget   ui;
	std::shared_ptr<CNewDealDlg> m_pNewDealDlg;
};
////////////////////////////////////////////////////////////////////////////////

///////////////////////// Implementing inline methods //////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
// class CTransactionsWidget
//

// Interface Methodes
inline std::shared_ptr<QSqlTableModel> CTransactionsWidget::GetTableModel()
{
	return nullptr;
}

// Helper Functions
inline void CTransactionsWidget::UpdateData(bool bFull)
{
	if (bFull)
	{
		QStringList lstCustomerNames( m_pCustomersData->GetCustomersName() );
		QStringList lstStorageNames( m_pStoragesData->GetStorageNames() );
		m_pNewDealDlg = std::shared_ptr<CNewDealDlg>(new CNewDealDlg( lstCustomerNames, lstStorageNames ));
		FM_CONNECT( ui.btnNewDeal, clicked(), m_pNewDealDlg.get(), show() );

		m_pStringListModel = std::shared_ptr<QStringListModel>( new QStringListModel( lstCustomerNames ) );

		ui.listView->setModel(m_pStringListModel.get());
		ui.listView->update();
	}

	if (m_strCurrentCustomerName == "")
		return;

	ui.tableView->setModel(m_pTransactionsData->GetSqlTableModelByCustomerName(m_strCurrentCustomerName).get());
	ui.tableView->update();
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////

#endif // GUI_TRANSACTIONS_WIDGET_H