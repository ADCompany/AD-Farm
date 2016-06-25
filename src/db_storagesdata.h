#ifndef DB_STORAGES_DATA_H
#define DB_STORAGES_DATA_H

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#ifndef DB_MANAGER_H
#   include "db_manager.h"
#endif

// Qt includes

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace fm {
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace db {
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// class CStoragesData
//
class CStoragesData : public CDBComponent
{
	Q_OBJECT
public:// Data Property

	class CPeriodicExpense // TODO...
	{
	public:// Constructor, Destructors
		CPeriodicExpense() = default;
		~CPeriodicExpense() = default;

	public:// Interface Methods


	public:// Contents
		int	ID;
		int	Frequency; // by day
		int	StorageID;

		double	Cost;

		QString InfoText;
		QString DateTime;
	};

	struct table
	{
		// tables name
		static const QString storage;
		static const QString storage_info;
		static const QString storage_history;
		static const QString storage_history_info;
		static const QString product;

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

	enum class EAction : int
	{
		None = -1,

		Buy = 0,
		Sell = 1,
		Decline = 2,

		Bring = 3,
		Refer = 4,
		Nourish = 5,

		Income = 6,
		Consumption = 7,

		PeriodicExpense = 8,

		All = None
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
		UpdatePeriodicExpenses();

		m_mapStringToModel.clear();
		m_mapStorageNameToStorageHistoryModel.clear();
		emit sigChangeData();
	}

	inline int GetColumnCount() const;
	QString GetLastGroupName();

	QString GetLastGroupNameByStorage(QString const& strStorageName);
	QStringList GetGroupNamesByStorage(QString const& strStorageName);
	QStringList GetGroupNames();

	double GetGroupIncome(QString const& strGroup);
	double GetGroupExpenses(QString const& strGroup);


	std::shared_ptr<QSqlQueryModel> GetProductSqlQueryModel(QString const& strTableName);
	std::shared_ptr<QSqlQueryModel> GetStorageSqlQueryModel(QString const& strTableName);

	int GetStorageScriptCount(QString const& strStorageName, QString const& strGroupName, EAction eAction) const;
	int GetStorageScriptCost(QString const& strStorageName, QString const& strGroupName, EAction eAction) const;

	double GetStorageTotalPrice(QString const& strStorageName, QString const& strGroupName) const;
	double GetStorageTotalPrice(QString const& strStorageName, QString const& strGroupName, EAction eAction) const;

	std::shared_ptr<QSqlQueryModel> GetSqlTableModelByStorageName(QString const& strStorageName);
	std::shared_ptr<QSqlQueryModel> GetHistorySqlTableModelByStorageName(QString const& strStorageName, QString const& strGroupName);
	std::shared_ptr<QSqlQueryModel> GetFarmHistorySqlTableModel();
	std::shared_ptr<QSqlQueryModel> GetGroupHistorySqlTableModel(QString const& strGroupName);

	void SubstractProducts(QList<QString> lstProductName, QList<int> lstProductCount);

	void BuyStorageData(QString const& strStorageName, QString const& strGroupName, QList<QString> lstProductNames, 
		QList<int> lstProductsCount, QList<double> lstProductCost, QString const& strInfoText);

	void AddNewGroup(QString const& strStorageName, QString const& strGroupName);
	void AddNewPeriodicExpense(double dCost, QString const& strInfoText, int nFrequency, QString const& strStorage);
	void AddNewProduct(QString const& strNewProductName, int nCount = 0, double dPrimeCost = 0.0);
	void AddNewStore(QString const& strStoreName);

	void NourishProductFromStorageToStorage(QString const& strInStorageName, QString const& strInStorageGroupName, QString strOutStorage, QString const& strOutStorageGroupName,
		QList<QString> const& lstProductName, QList<int> const& lstProductCount, QString const& strInStorageInfoText, QString const& strOutStorageInfoText);
	void MoveProductFromStorageInStorage(QString const& strInStorageName, QString const& strInStorageGroupName, QString strOutStorage, QString const& strOutStorageGroupName,
		QList<QString> const& lstProductName, QList<int> const& lstProductCount, QString const& strInStorageInfoText, QString const& strOutStorageInfoText);

	void AddProductInStorage(QString const& strStorageName, QString const& strGroupName, QList<QString> const& lstProductName,
		QList<int> const& lstProductCount, QList<double> const& lstProductCost, QString const& strInfoText);

	void SubstractProductInStorage(QString const& strStorageName, QString const& strProductName, int nCount);
	void DeclineProductInStorage(QString const& strStorageName, QString const& strGroupName, QString const& strProductName, int nCount, QString const& strInfoText);

	void AddFarmCosts(double dCosts, QString const& strInfoText);
	void AddStoragesCosts(QString const& strStorageName, QString const& strGroupName, double dCosts,
		QString const& strInfoText, QString const& strStorageInfoText = "", EAction eAction = EAction::Consumption);

	QList<QString> GetStorageNames();

	QList<QString> GetProductNames();

protected:// Helper Methodes
	void UpdatePeriodicExpenses();

	void UpdateSqlTableModel();
	void UpdateSqlTableModel(QString const& strStorageName);
	void RemoveSqlTableModel(QString const& strStorageName);

	inline QString GetStorageIdByName(QString const& strStorageName) const;
	inline QString GetStorageNameById(QString const& strStorageID) const;
	inline QString GetStorageNameById(int nStorageID) const;

	inline QString GetGroupIdByName(QString const& strGroupName) const;
	inline QString GetGroupNameById(QString const& strGroupID) const;
	inline QString GetGroupNameById(int nGroupID) const;

signals:
	void sigChangeData();

private:// Members
	// ?? stack
	std::map< QString, std::shared_ptr<QSqlQueryModel> > m_mapStringToModel;
	std::map< QString, std::shared_ptr<QSqlQueryModel> > m_mapStorageNameToStorageHistoryModel;

	std::shared_ptr<QSqlQueryModel> m_pGroupSqlTableModel;
	std::shared_ptr<QSqlQueryModel> m_pFarmHistorySqlTableModel;

	QObject* m_pParentObject;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////// Implementing inline methods //////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// class CStoragesData
//

// Constructors
inline CStoragesData::CStoragesData(QObject* pParent, std::shared_ptr<CDBManager> pDBManager)
	: CDBComponent(pDBManager),
	m_pParentObject(pParent),
	m_pGroupSqlTableModel(nullptr),
	m_pFarmHistorySqlTableModel(nullptr)
{}

// Interface Methodes
inline int CStoragesData::GetColumnCount() const
{

	return 0;
}

// Helper Methods
inline QString CStoragesData::GetStorageIdByName(QString const& strStorageName) const
{
	// FM_ASSERT_EX(strStorageName != "", "Empty storage name");

	QSqlQuery sqlQuery;
	sqlQuery.exec(QString("SELECT id FROM %1 WHERE name == \"%2\"").arg(CStoragesData::table::storage, strStorageName));
	sqlQuery.next();

	return sqlQuery.value(0).toString();
}

inline QString CStoragesData::GetStorageNameById(QString const& strStorageID) const
{
	// FM_ASSERT_EX(strStorageID != "", "Empty storage id");

	QSqlQuery sqlQuery;
	sqlQuery.exec(QString("SELECT name FROM %1 WHERE id == %2").arg(CStoragesData::table::storage, strStorageID));
	sqlQuery.next();

	return sqlQuery.value(0).toString();
}

inline QString CStoragesData::GetStorageNameById(int nStorageID) const
{
	// FM_ASSERT_EX(nStorageID != -1, "Empty storage id");

	QString strStorageID = QString::number(nStorageID);

	return GetStorageNameById(strStorageID);
}

inline QString CStoragesData::GetGroupIdByName(QString const& strGroupName) const
{
	// FM_ASSERT_EX(strGroupName != "", "Empty storage name");

	QSqlQuery sqlQuery;
	sqlQuery.exec(QString("SELECT id FROM group_name WHERE name == \"%1\"").arg(strGroupName));
	sqlQuery.next();

	return sqlQuery.value(0).toString();
}

inline QString CStoragesData::GetGroupNameById(QString const& strGroupID) const
{
	// FM_ASSERT_EX(strGroupID != "", "Empty group id");

	QSqlQuery sqlQuery;
	sqlQuery.exec(QString("SELECT name FROM group_name WHERE id == %1").arg(strGroupID));
	sqlQuery.next();

	return sqlQuery.value(0).toString();
}

inline QString CStoragesData::GetGroupNameById(int nGroupID) const
{
	// FM_ASSERT_EX(nGroupID != -1, "Empty group id");

	QString strStorageID = QString::number(nGroupID);

	return GetGroupNameById(strStorageID);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace db
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // DB_STORAGES_DATA_H