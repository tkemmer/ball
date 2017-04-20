#include <BALL/PYTHON/pyJupyterKernel.h>

#include <BALL/COMMON/logStream.h>

using std::string;
using std::tie;

namespace BALL
{
	PyJupyterKernel::PyJupyterKernel() :
		PyKernel{},
		kernel_{},
		connection_file_{}
	{
		if (!kernel_.isStarted()) return;

		bool ok;
		string res;

		// TODO check
		kernel_.run("import ball_jupyter_kernel as bjc");

		// TODO check
		kernel_.run("__BALL_KERNEL = bjc.BALLJupyterKernel()");

		// TODO check
		tie(ok, connection_file_) = kernel_.run("sys.stdout.write(__BALL_KERNEL.client.connection_file)");
		Log.info() << "Connection file: " << connection_file_ << '\n'; // TODO remove

		// Also add BALL-specific script path to Jupyter kernel
		loadScriptDir();
		run("import ball_jupyter_kernel as bjc");
	}

	PyJupyterKernel::~PyJupyterKernel()
	{
		kernel_.run("__BALL_KERNEL.stop_kernel()");
	}

	bool PyJupyterKernel::isStarted() const
	{
		return kernel_.isStarted(); // TODO
	}

	string PyJupyterKernel::getErrorMessage() const
	{
		return kernel_.getErrorMessage(); // TODO
	}

	std::pair<bool, string> PyJupyterKernel::run(std::string str)
	{
		// TODO remove multi-line string
		kernel_.run(string("__BALL_CMD = \"\"\"") + str + "\"\"\"");

		// TODO error handling
		return kernel_.run("sys.stdout.write(__BALL_KERNEL.execute(__BALL_CMD))");
	}

	bool PyJupyterKernel::runFile(std::string /* filename */)
	{
		Log.error() << "[PyJupyterKernel] This kernel does not support executing files" << std::endl;
		return false;
	}

	bool PyJupyterKernel::execute(const std::string& module, const std::string& func, const KeyValArgs& args)
	{
		Log.info () << "[PyJupyterKernel] Executing..." << std::endl;
		bool ok;
		string res;

		tie(ok, res) = run(string("bjc.call_function('") + module + "', '" + func + "', '')");

		Log.error() << ok << ": " << res << std::endl;

		return ok;
		/*
		if (!loadModule(module))
		{
			Log.error() << "[PyJupyterKernel] Could not load module " << module << '\n';
			return false;
		}

		// TODO
		//return kernel_.execute(module, func, args);
		return false;
		 */
	}

	bool PyJupyterKernel::loadModule(const std::string& module)
	{
		bool ok;
		string res;
		tie(ok, res) = run(module + " in sys.modules");
		if (ok && res == "True") tie(ok, res) = run(string("import ") + module);
		return ok;
	}
}