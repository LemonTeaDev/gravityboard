template<typename T>
class Singleton
{
public:
	T& Instance()
	{
		static T instance;
		return instance;
	}
};