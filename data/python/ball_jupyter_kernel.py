import jupyter_client as jc
import importlib, sys, zmq

class BALLJupyterKernel(object):
	"""
	TODO
	"""
	def __init__(self, autostart=True):
		"""
		TODO
		"""
		if autostart:
			self.start_kernel()
	
	
	def start_kernel(self):
		"""
		TODO
		"""
		self.manager = jc.KernelManager()
		self.manager.start_kernel()
		self.client = self.manager.client()
		

	def stop_kernel(self):
		"""
		TODO
		"""
		try:
			self.client.shutdown()
			self.manager.shutdown_kernel()
		except:
			pass


	def execute(self, exec_str, timeout = 10):
		"""
		TODO
		
		source
		"""
		msg_id = self.client.execute(exec_str, allow_stdin = False)
		
		poller = zmq.Poller()
		poller.register(self.client.iopub_channel.socket, zmq.POLLIN)
		
		ret = ""
		while True:
			events = dict(poller.poll(1000 * timeout))
			if self.client.iopub_channel.socket not in events:
				continue
			
			msg = self.client.iopub_channel.get_msg(timeout = 0)

			# ignore messages that are not addressed to us
			if not msg['parent_header'].get('msg_id', '') == msg_id:
				continue
			
			msg_type    = msg['header']['msg_type']
			msg_content = msg['content']
			
			# normal callback
			if msg_type in ('display_data', 'execute_result'):
				ret += msg_content['data'].get('text/plain', '')
				
			# execution error
			elif msg_type == 'error':
				ret += '\n'.join(msg_content['traceback'])
			
			# execution has finished
			elif msg_type == 'status' and \
			msg_content['execution_state'] == 'idle':
				break
		return ret


def call_function(module_name, func_name, args):
	"""
	TODO
	"""
	
	# get module
	module = sys.modules.get(module_name, importlib.import_module(module_name))

	# get function from module
	func = getattr(module, func_name)
		
	func('/home/thomas/git/apps/ball/data/projects/1FK9_no_mut_2.bvp')
		
		
		
		
		
		
		
		
		
		
		
