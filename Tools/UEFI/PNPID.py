import xml.etree.ElementTree as ET
from bs4 import BeautifulSoup
from urllib.parse import quote
import time

class PNPIDResquest:
    __root  = None
    def __init__(self):
        self.__root = ET.Element('PNPIDDatabase',attrib={'version':'1.0','url':r'https://uefi.org/pnp_id_list','BuildDate':time.strftime("%Y-%m-%d", time.localtime())})

    def SaveRoot(self):
        tree = ET.ElementTree(self.__root)
        tree.write("EDIDDatabase.xml",encoding="utf-8",xml_declaration=True)
    def BuildQuery(self):
        with open(r'pnp_id_registry_content_.xls', 'r',encoding='utf-8') as f:
            soup=BeautifulSoup(f.read(),'lxml')
            table = soup.find('table')
            rows = table.find_all('tr')
            for row in rows:
                cols = row.find_all('td')
                if not cols:
                    continue
                cols = [ele.text.strip() for ele in cols]
                self.InsertElement([ele for ele in cols if ele])
        return True
    def InsertElement(self, ele):
        PnpId = ET.SubElement(self.__root,"PNPID", attrib={'Manufacture': ele[0].strip()})
        PnpId.text = ele[1].strip()

if __name__ == '__main__':
    resq = PNPIDResquest()
    if resq.BuildQuery():
        print('Generating')
        resq.SaveRoot()
        print('Generated')
    else:
       print('error to query cpu info from cpudb')
    