import sys
import datetime
import re

K_AUTHORS = ["Leonardo Cascianelli", "Rory Brown", "Ewan Skene"]
CURR_DATE = datetime.datetime.now()
DATE_STRING = str(CURR_DATE.year)+'-'+str(CURR_DATE.month)+'-'+str(CURR_DATE.day)

class Classtructor(object):
	def __init__(self):
		self.name = ""
		self.description = ""

	def set_class_name(self, name):
		self.name = name

	def set_description(self, description):
		self.description = description


	def run(self):
		if not self.validate():
			print("[!!] Class invalid, arguments missing.")
			exit(-1)

		self.build_class_header()
		self.build_class_source()
		self.add_to_umbrella()

	def build_class_header(self):

		with open("./oop_templates/Class_Template.h",'r') as template:
			with open(self.name+".h",'w') as header:
				temp_content = template.read()
				temp_content = temp_content.replace("__CLASS_NAME__",self.name)
				temp_content = temp_content.replace("__DATE__",DATE_STRING)
				if (self.description != ""):
					temp_content = temp_content.replace("__DESCRIPTION__",self.description)
				temp_content = temp_content.replace("__AUTHORS__",', '.join(K_AUTHORS))
				header.write(temp_content)

	def build_class_source(self):
		
		with open("./oop_templates/Class_Template.c",'r') as template:		
			with open(self.name+".c",'w') as source:
				temp_content = template.read()
				temp_content = temp_content.replace("__CLASS_NAME__",self.name)
				temp_content = temp_content.replace("__DATE__",DATE_STRING)
				if (self.description != ""):
					temp_content = temp_content.replace("__DESCRIPTION__",self.description)
				temp_content = temp_content.replace("__AUTHORS__",','.join(K_AUTHORS))
				source.write(temp_content)

	def add_to_umbrella(self):
		lines = []
		with open("umbrella.h","r") as umbrella:
			lines = umbrella.readlines()
		with open("umbrella.h","w") as umbrella:
			lines.insert(-2, '#include "'+self.name+'.h"\n')
			umbrella.write("".join(lines) if (lines[-1] == '\n') else "".join(lines))


	def validate(self):
		return self.name != ""

def main(argv):
	constructor = Classtructor()

	available_flags = { ('-n','--name'): constructor.set_class_name, 
						('-d','--description'): constructor.set_description}

	for flag_tuple in available_flags:
		for index, arg in enumerate(argv):
			if arg in flag_tuple:
				if index < len(argv) - 1:
					available_flags[flag_tuple](argv[index+1])
				else:
					print(f"No argument found for {argv[index]}")
					exit(0)

	constructor.run()

if __name__ == '__main__':
	main(sys.argv[1:])
