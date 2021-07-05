import xml.etree.ElementTree as ET
import time
import re

class MemoryRequest:
    __root = None
    def __init__(self):
        self.__root = ET.Element('MemoryDatabase',attrib={'version':'1.0','source':r'JEP106BC','BuildDate':time.strftime("%Y-%m-%d", time.localtime())})
    def BuildQuery(self):
        for id in range(1,14,1):
            file = open(r'JEP106{}.txt'.format(id), 'r', encoding='utf-8')
            bank = ET.SubElement(self.__root, 'Bank', attrib={'id': str(id)})
            try:
                text_lines = file.readlines()
                for line in text_lines:
                    self.InsertElement(bank,line)
            finally:
                file.close()
        return True
        
    def SaveRoot(self):
        tree = ET.ElementTree(self.__root)
        tree.write("MemoryDatabase.xml",encoding="utf-8",xml_declaration=True)

    def InsertElement(self, bank, line):
        textline = re.sub('\s[0|1]\s', ' ', re.sub('\s[0|1]\s', ' ', re.sub('\s[0|1]\s', ' ', re.sub('\s[0|1]\s', ' ', line))))
        text = textline[textline.find(' ', 0, len(textline)):textline.rfind(' ', 0 , len(textline))]
        id = textline[textline.rfind(' ', 0, len(textline)):]
        ele = ET.SubElement(bank,'Manufacture',attrib={'id' : str(int(id.strip(), 16))})
        ele.text = text.strip()

if __name__ == '__main__':
    resq = MemoryRequest()
    if resq.BuildQuery():
        print('Generating')
        resq.SaveRoot()
        print('Generated')
    else:
       print('error to query cpu info from cpudb')