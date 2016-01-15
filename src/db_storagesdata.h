#ifndef DB_STORAGES_DATA_H
#define DB_STORAGES_DATA_H

////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#ifndef DB_MANAGER_H
#   include "db_manager.h"
#endif

// Qt includes

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace fm {
////////////////////////////////////////////////////////////////////////////////
namespace db {
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
// class CStoragesData
//
class CStoragesData : public CDBComponent
{
	Q_OBJECT
public:// Data Property
	struct table
	{
		// tables name
		static const QString storage;
		static const QString storage_info;
		static const QString storage_history;
		static const QString storage_history_info;
		static const QString producte;

		// table customer
		//struct storage
		//{
		//	static const QString id;
		//	static const QString name;
		//};
		//struct storage_info
		//{
		//	static const QString storage_id;
		//	static const QString product_id;
		//	static const QString count;
		//	static const QString prime_cost;
		//};
		//struct storage_history
		//{
		//	static const QString id;
		//	static const QString storage_id;
		//	static const QString date_time;
		//};
		//struct storage_history_info
		//{
		//	static const QString history_id;
		//	static const QString product_id;
		//	static const QString count;
		//	static const QString cost;
		//};
		//struct product
		//{
		//	static const QString id;
		//	static const QString name;
		//	static const QString prime_cost;
		//};
	};
public:// Constructors
	inline CStoragesData(QObject* pParent = nullptr, std::shared_ptr<CDBManager> pDBManager = nullptr);
	~CStoragesData() = default;

public:// Interface Methodes
	void Initialize();

	bool SetParent(QObject* pParent);
	void SetDBManager(std::shared_ptr<CDBManager> pDBManager);

	void RemoveProduct(int nRow);
	void RemoveStorage(int nRow);

	void UpdateAllSqlTableModel()
	{
		m_mapStringToModel.clear();
		m_mapStorageNameToStorageHistoryModel.clear();
		emit sigChangeData();
	}

	inline int GetColumnCount() const;

	std::shared_ptr<QSqlQueryModel> GetProductSqlQueryModel(QString const& strTableName);
	std::shared_ptr<QSqlQueryModel> GetStorageSqlQueryModel(QString const& strTableName);

	std::shared_ptr<QSqlQueryModel> GetSqlTableModelByStorageName(QString const& strStorageName);
	std::shared_ptr<QSqlQueryModel> GetHistorySqlTableModelByStorageName(QString const& strStorageName);
	std::shared_ptr<QSqlQueryModel> GetFarmHistorySqlTableModel();

	void SubstractProducts(QList<QString> lstProductName, QList<int> lstProductCount);

	void BuyStorageData(QString const& strStorageName, QList<QString> lstProducteNames, QList<int> lstProductesCount, QList<double> lstProductCost, QString const& strInfoText);

	void AddNewProduct(QString const& strNewProductName, int nCount, double dPrimeCost);
	void AddNewStore(QString const& strStoreName);

	void MoveProductFromStorageInStorage(QString const& strInStorageName, QString strOutStorage,
		QList<QString> const& lstProductName, QList<int> const& lstProductCount, QString const& strInStorageInfoText, QString const& strOutStorageInfoText);

	void AddProductInStorage(QString const& strStorageName, QList<QString> const& lstProductName,
		QList<int> const& lstProductCount, QList<double> const& lstProductCost, QString const& strInfoText);

	void SubstractProductInStorage(QString const& strStorageName, QString const& strProductName, int nCount);
	void DeclineProductInStorage(QString const& strStorageName, QString const& strProductName, int nCount, QString const& strInfoText);

	void AddFarmCosts(double dCosts, QString const& strInfoText);
	void AddStoragesCosts(QString const& strStorageName, double dCosts, QString const& strInfoText);

	QList<QString> GetStorageNames();

	QList<QString> GetProductNames();

protected:// Helper Methodes
	void UpdateSqlTableModel();
	void UpdateSqlTableModel(QString const& strStorageName);
	void RemoveSqlTableModel(QString const& strStorageName);

signals:
	void sigChangeData();

private:// Members
	std::map< QString, std::shared_ptr<QSqlQueryModel> > m_mapStringToModel;
	std::map< QString, std::shared_ptr<QSqlQueryModel> > m_mapStorageNameToStorageHistoryModel;

	std::shared_ptr<QSqlQueryModel> m_pFarmHistorySqlTableModel;

	QObject* m_pParentObject;
};
////////////////////////////////////////////////////////////////////////////////

///////////////////////// Implementing inline methods //////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
// class CStoragesData
//

// Constructors
inline CStoragesData::CStoragesData(QObject* pParent, std::shared_ptr<CDBManager> pDBManager)
	: CDBComponent(pDBManager),
	m_pParentObject(pParent),
	m_pFarmHistorySqlTableModel(nullptr)
{}

// Interface Methodes
inline int CStoragesData::GetColumnCount() const
{

	return 0;
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace db
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////

#endif // DB_STORAGES_DATA_H