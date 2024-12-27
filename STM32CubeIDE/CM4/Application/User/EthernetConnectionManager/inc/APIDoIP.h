#include <optional>
#include <tuple>
#include "MessageDataTypes.h"


class DoIPAPI
{
public:
	DoIPAPI(){}
	~DoIPAPI(){}

	std::optional<SMessage> processReceivedData(char* data[], size_t len);
	void generateDoIPQuery(unsigned int functionCode, char** generatedQuery, size_t queryLen);

protected:



};
