#ifndef LISTENERPJ_H
#define LISTENERPJ_H

namespace pj{
template<class Tipo>
class Listener
{
	public:
		virtual void notify ( Tipo ) = 0;
	protected:
	private:
};
}
#endif // LISTENER_H
