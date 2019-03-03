#ifndef SERVER_H_
#define SERVER_H_


#include "common/Singleton.h"
#include "service/IProcess.h"

class Service : public Singleton<Service>
{
public:
	Service();
	~Service();

	int32_t ServiceType();

	void Start(IProcessFactoryPtr process_factory, int32_t service_type);

	void Loop();
};


#endif
