class chatUser
{
private:
	int cUID;
	int socketID;
	const char *uName;
public:
	int setCUID(int cUIDSet)
	{
		cUID = cUIDSet;
	}
	int setSocketID(int socketIDSet)
	{
		socketID = socketIDSet;
	}
	const char *setUName(const char *uNameSet)
	{
		uName = uNameSet;
	}
	int getCUID()
	{
		return cUID;
	}
	int getSocketID()
	{
		return socketID;
	}
	const char *getUName()
	{
		return uName;
	}
};
