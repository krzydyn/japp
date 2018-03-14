#include <lang/Number.hpp>
#include <lang/System.hpp>

int main() {
	Boolean t1(true);
	Short t2(1);
	Integer t3(1);
	Long t4(1);
	Boolean::compare(true,false);
	int i = Integer::parseInt("123");
	if (i != 123) LOGE("parseInt error");
	i = Integer::parseInt("A23");
	return 0;
}
