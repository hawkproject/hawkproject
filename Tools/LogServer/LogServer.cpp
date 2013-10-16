#include "HawkUtil.h"
#include "HawkLog.h"

int main(int argc, Char* argv[])
{
	HawkUtil::Init();

	AString sCfg = "LogServer.xml";
	if (argc >= 2)
		sCfg = argv[1];

	AXmlDocument doc;
	HawkXmlFile  xml;
	if (!xml.Open(sCfg, doc))
		HawkFmtPrint("LogServer Open XmlCfg Error.");

	AString sAddr = "*:9595";
	if (doc.GetRoot()->GetAttribute("Addr"))
		sAddr = doc.GetRoot()->GetAttribute("Addr")->StringValue();

	Bool bConsole = false;
	if (doc.GetRoot()->GetAttribute("Console"))
		bConsole = doc.GetRoot()->GetAttribute("Console")->BoolValue();

	Int32 iCacheSize = MEGABYTE;
	if (doc.GetRoot()->GetAttribute("CacheSize"))
		iCacheSize = doc.GetRoot()->GetAttribute("CacheSize")->IntValue();

	Int32 iCacheTime = 60000;
	if (doc.GetRoot()->GetAttribute("CacheTime"))
		iCacheTime = doc.GetRoot()->GetAttribute("CacheTime")->IntValue();

	HawkLogServer* pLogSvr = new HawkLogServer;

	if (doc.GetRoot()->GetChildAttribute("LogFile", "Valid") && 
		doc.GetRoot()->GetChildAttribute("LogFile", "File") &&
		doc.GetRoot()->GetChildAttribute("LogFile", "Valid")->BoolValue())
	{
		AString sLogFile = doc.GetRoot()->GetChildAttribute("LogFile", "File")->StringValue();

		if (!pLogSvr->Init(sAddr, sLogFile, iCacheSize, iCacheTime))
			HawkFmtPrint("LogServer Init Error.");
	}
	else if (doc.GetRoot()->GetChildAttribute("LogDB", "Valid") && 
		doc.GetRoot()->GetChildAttribute("LogDB", "DB") &&
		doc.GetRoot()->GetChildAttribute("LogDB", "Valid")->BoolValue())
	{
		HawkDBConn sConn;
		if (doc.GetRoot()->GetChildAttribute("LogDB", "IP"))
			sConn.SetHost(doc.GetRoot()->GetChildAttribute("LogDB", "IP")->StringValue());

		if (doc.GetRoot()->GetChildAttribute("LogDB", "Port"))
			sConn.SetPort(doc.GetRoot()->GetChildAttribute("LogDB", "Port")->IntValue());

		if (doc.GetRoot()->GetChildAttribute("LogDB", "User"))
			sConn.SetUser(doc.GetRoot()->GetChildAttribute("LogDB", "User")->StringValue());

		if (doc.GetRoot()->GetChildAttribute("LogDB", "Pwd"))
			sConn.SetPwd(doc.GetRoot()->GetChildAttribute("LogDB", "Pwd")->StringValue());

		if (doc.GetRoot()->GetChildAttribute("LogDB", "DB"))
			sConn.SetDBName(doc.GetRoot()->GetChildAttribute("LogDB", "DB")->StringValue());

		if (sConn.GetHost().size() || sConn.GetPwd().size() || sConn.GetPort() > 0)
			sConn.SetType(HawkDatabase::HDB_MYSQL);
		else
			sConn.SetType(HawkDatabase::HDB_SQLITE);

		if (!pLogSvr->Init(sAddr, sConn, iCacheSize, iCacheTime))
			HawkFmtPrint("LogServer Init Error.");
	}
	
	if (!pLogSvr->Run())
		HawkFmtPrint("LogServer Run Error.");

	HAWK_RELEASE(pLogSvr);

	HawkUtil::Stop();
	HawkUtil::Release();
	return 0;
}

