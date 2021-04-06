import requests
import xml.etree.ElementTree as ET
from bs4 import BeautifulSoup
from urllib.parse import quote
import time
import random
import json

def buildUrl(generation):
    url = "https://www.techpowerup.com/cpu-specs/?generation={}&sort=released&ajax=1".format(quote(generation))
    return url

class CPUDBResquest:
    query=[]
    data=[]
    __root  = None
    __Intel = None
    __AMD = None
    __ZhaoXin = None
    def __init__(self):
        self.__root = ET.Element('CPUDatabase',attrib={'version':'1.0','url':r'https://www.techpowerup.com/cpu-specs','BuildDate':time.strftime("%Y-%m-%d", time.localtime())})
        self.__Intel = ET.SubElement(self.__root,"Manufacture",attrib={'name': 'Intel'})
        self.__AMD = ET.SubElement(self.__root,"Manufacture",attrib={'name': 'AMD'})
        self.__ZhaoXin = ET.SubElement(self.__root,"Manufacture",attrib={'name': 'Zhaoxin'})

    def SaveRoot(self):
        tree = ET.ElementTree(self.__root)
        tree.write("CPUDatabase.xml",encoding="utf-8",xml_declaration=True)

    def __BuildXmlFromData(self, Manufacture, FamilyName):  
        if len(self.data) == 0:
            return
        Family = ET.SubElement(Manufacture,"Family", attrib={'name': FamilyName})
        for cpu in self.data:
            if len(cpu) == 9:
                CPU = ET.SubElement(Family,'CPU',attrib={'name':cpu[0].replace(FamilyName,'').replace('-','').strip()})

                CodeName = ET.SubElement(CPU,'CodeName')
                CodeName.text = cpu[1].strip()  

                SocketPackage = ET.SubElement(CPU,'SocketPackage')
                SocketPackage.text = cpu[4].strip()  

                Technology = ET.SubElement(CPU,'Technology')
                Technology.text = cpu[5].replace(' nm','').strip()  

                MaxTDP = ET.SubElement(CPU,'MaxTDP')
                MaxTDP.text = cpu[7].replace(' W','').strip()  

                Released = ET.SubElement(CPU,'ReleaseDate')
                Released.text = cpu[8].strip()

    def BuildQuery(self):
        r=requests.get(r'https://www.techpowerup.com/cpu-specs',headers={'user-agent':'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/89.0.4389.90 Safari/537.36'})
        if r.status_code != 200:
            return False
        soup=BeautifulSoup(r.text,'lxml')
        article = soup.select('#page > article')
        ul = article[0].select('#form > section > div > fieldset.filters > ul')
        for li in ul[0].find_all('li'):
            select = li.find('select',attrs={'id':'generation','name':'generation'})
            if select is not None:
                for option in select.find_all('option'):
                    print(option['value'])
                    if len(option['value']) > 0:
                        self.query.append(option['value'])
        return True

    def QueryFromCPUDB(self,generation):
        count = 1
        time.sleep(60)
        while count <= 3 :
            url = buildUrl(generation)
            r=requests.get(url,headers={'user-agent':'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/89.0.4389.90 Safari/537.36'})
            if r.status_code != 200:
                print('Request: {0} Http Error: {1}'.format(url, r.status_code))
                time.sleep(60*5 * count)
                count += 1
            else:
                return r
        return None

    def BuildCPUDBFromQuery(self):
        for generation in self.query:
            print('get {} information'.format(generation))
            r = self.QueryFromCPUDB(generation)
            if r is None:
                break
            response = json.loads(r.text)
            if response['list'] is not None:
                soup = BeautifulSoup(response['list'],'lxml')
                table = soup.find('table',class_='processors')
                rows = table.find_all('tr')
                for row in rows:
                    cols = row.find_all('td')
                    cols = [ele.text.strip() for ele in cols]
                    self.data.append([ele for ele in cols if ele]) # Get rid of empty values
                self.data = list(filter(None, self.data))
                if generation.find('Intel') != -1:
                    self.__BuildXmlFromData(self.__Intel, generation[5:].strip())
                elif generation.find('AMD') != -1:
                    self.__BuildXmlFromData(self.__AMD, generation[3:].strip())
            self.data = []
            print('Finished get {} information'.format(generation))

if __name__ == '__main__':
    resq = CPUDBResquest()
    if resq.BuildQuery():
        print('Generating')
        resq.BuildCPUDBFromQuery()
        resq.SaveRoot()
        print('Generated')
    else:
        print('error to query cpu info from cpudb')

