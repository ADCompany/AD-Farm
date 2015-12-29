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

	std::shared_ptr<QDialog> GetAddTransactionDialog()
	{
		return m_pNewDealDlg;
	}

protected:// Helper Methodes
	inline void UpdateData(bool bFull = false);


protected slots:// Slots
	void onChangeData()
	{
		UpdateData(true);
	}
	void onChangeDataByTransactionData()
	{
		UpdateData();
	}
	void onAddTransaction()
	{
		// TRDATA
		STransactionDetails trDetails = m_pNewDealDlg->GetDealDetails();

		QString strCustomerName = trDetails.sCustomerName;
		QList<QString> lstProductName;
		QList<int> lstCount;
		QList<double> lstCost;
		for (int i = 0; i < trDetails.lstProductInfo.count(); ++i)
		{
			lstProductName.push_back(trDetails.lstProductInfo[i].sProductName);
			lstCount.push_back(trDetails.lstProductInfo[i].nCount);
			lstCost.push_back(trDetails.lstProductInfo[i].fPrice);
		}


		m_pTransactionsData->AddTransactionData(strCustomerName, lstProductName, lstCount, lstCost, trDetails.fTotalPrice, trDetails.fPayedMoney);

		// PRDATA
		m_pStoragesData->SubstractProducts(lstProductName, lstCount);

		// STDATA
		CFarmInfo farmInfo = m_pNewDealDlg->GetFarmUpdateInfo();

		for (int i = 0; i < farmInfo.count(); ++i)
		{
			QString strStorageName = farmInfo[i].sStoreageName;
			QList<int> lstPrCount;
			QList<QString> lstProducts;
			for (int j = 0; j < farmInfo[i].lstProducts.count(); ++j)
			{
				lstProducts.push_back(farmInfo[i].lstProducts[j].sName);
				lstPrCount.push_back(farmInfo[i].lstProducts[j].nCount);
			}
			m_pStoragesData->BuyStorageData(strStorageName, lstProducts, lstPrCount);
		}

		QString strFirstName = "";
		QString strLastName = "";
		int nIndex = 0;
		for (; nIndex< strCustomerName.size(); ++nIndex)
		{
			if (strCustomerName[nIndex] == ' ')
				break;
			strFirstName += strCustomerName[nIndex];
		}
		++nIndex;
		for (; nIndex < strCustomerName.size(); ++nIndex)
		{
			strLastName += strCustomerName[nIndex];
		}
		m_pCustomersData->UpdateCustomerDebt(strFirstName, strLastName, trDetails.fTotalPrice - trDetails.fPayedMoney);
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
		FM_CONNECT(ui.btnNewDeal, clicked(), m_pNewDealDlg.get(), show());
		FM_CONNECT(m_pNewDealDlg.get(), accepted(), this, onAddTransaction());

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