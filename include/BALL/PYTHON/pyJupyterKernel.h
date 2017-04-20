#ifndef BALL_PYTHON_PYJUPYTERKERNEL_H
#define BALL_PYTHON_PYJUPYTERKERNEL_H

#include <BALL/PYTHON/pyCAPIKernel.h>

namespace BALL
{
	class PyJupyterKernel : public PyKernel
	{
		public:
			PyJupyterKernel();
			~PyJupyterKernel();

			std::string getConnectionFile() const { return connection_file_; }

			bool isStarted() const override;
			std::string getErrorMessage() const override;

			std::pair<bool, std::string> run(std::string str) override;
			BALL_DEPRECATED bool runFile(std::string filename) override;
			bool execute(const std::string& module, const std::string& func, const KeyValArgs& args) override;

		private:
			bool loadModule(const std::string& name);

			PyCAPIKernel kernel_;
			std::string connection_file_;
	};
}

#endif // BALL_PYTHON_PYJUPYTERKERNEL_H
