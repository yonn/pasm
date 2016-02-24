#ifndef PASM_LABEL_HPP
#define PASM_LABEL_HPP

namespace pasm {

	struct Label {
		int home_line;
		int ptr_line;

		Label(int h) : home_line(h) { }
		Label() { }
	};

}

#endif /*PASM_LABEL_HPP*/
