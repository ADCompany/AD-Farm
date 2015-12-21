/******************************************************************************/
/*  This is the open source project which has GNU GNU GENERAL PUBLIC LICENSE  */
/*	        The developers are Aram Hakhverdyan and Levon Gasparyan.          */
/******************************************************************************/

#ifndef FM_CORE_H
#define FM_CORE_H

////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//

//	Qt includes
#include <QObject>
#include <QMetaType>
#include <QPair>
#include <QString>
#include <QList>
#include <QMap>
#include <QVector>
#include <QSet>
#include <QHash>
#include <QStack>
#include <QQueue>
#include <QByteArray>
#include <QBitArray>
#include <QLinkedList>
#include <QStringList>
#include <QVariant>
#include <QDateTime>
#include <QDataStream>
#include <QTextStream>
#include <QSharedPointer>
#include <QScopedPointer>
#include <QCoreApplication>
#include <QtCore/qglobal.h>

//  STD includes
#include <stdexcept>
#include <new>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	Macros Helpers
//
#if defined (QT_DEBUG)
#	define FM_ASSERT(_test_) Q_ASSERT(_test_)
#   define FM_ASSERT_EX( _test_, _what_ ) Q_ASSERT_X( _test_, '\0', _what_ )
#elif defined (QT_NO_DEBUG)
#	define FM_ASSERT(_test_)             ((void)(0))
#	define FM_ASSERT_X( _test_, _what_ ) ((void)(0))
#endif
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	Library Export
//
//#if defined FM_CORE_EXPORT
//#	undef FM_CORE_EXPORT
//#endif
//#ifdef FM_CORE_LIB
//#	define FM_CORE_EXPORT Q_DECL_EXPORT
//#else
//#	define FM_CORE_EXPORT Q_DECL_IMPORT
//#endif
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace fm {
////////////////////////////////////////////////////////////////////////////////
namespace keys {
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
} // namespace keys
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
// QTR function declaration for internationalization
//
inline QString qtr(char const* cszText)
{
	return QCoreApplication::translate("fm_core", cszText);
}
inline QString qtr(QLatin1String const& cszText)
{
	return qtr(cszText.latin1());
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CException
//  
class CException : public std::exception
{
public:// Constructors
	inline CException(QString const& sErr = qtr(m_cszDefaultMsg))	throw();
	inline CException(CException const& rhs) throw();
	virtual ~CException() = default;

public:// Interface Methodes
	inline CException& operator=(CException const& rhs) throw();
	const char* what() const throw() override;

private:// Members
	QByteArray m_aErrMsg;
	static const QLatin1String m_cszDefaultMsg;
};
////////////////////////////////////////////////////////////////////////////////

///////////////////////// Implementing inline methods //////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CException
//  

// Constructors
inline CException::CException(QString const& sErr)	throw()
	: m_aErrMsg(sErr.toUtf8())
{}

inline CException::CException(CException const& rhs) throw()
	: m_aErrMsg(rhs.m_aErrMsg)
{}

// Interface Methodes
inline CException& CException::operator=(CException const& rhs) throw()
{
	m_aErrMsg = rhs.m_aErrMsg;
	return *this;
}

inline const char* CException::what() const throw()
{
	return m_aErrMsg.constData();
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////

#endif // FM_CORE_H