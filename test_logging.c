#include "opencstl.h"


int main() {
    MsgBoxCLI("Logging library: %s", opencstl_version());
    logging.debug("debug logging message");
    logging.info("info logging message");
    logging.warning("warning logging message");
    logging.error("error logging message");
    logging.critical("critical logging message");
    logging.fatal("fatal logging message");
    MsgBoxGUI("%d / 0  - divide by zero", 777);
}
