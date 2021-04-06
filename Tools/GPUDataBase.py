import requests
import xml.etree.ElementTree as ET
from bs4 import BeautifulSoup
from urllib.parse import quote
import time
import random
import json

class GPUDBResquest:
    query=[]
    data=[]
    __root  = None
    __url = None
    def __init__(self, url):
        self.__root = ET.Element('GPUDatabase',attrib={'version':'1.0','url':url,'BuildDate':time.strftime("%Y-%m-%d", time.localtime())})
        self.__url = url

    def SaveRoot(self):
        tree = ET.ElementTree(self.__root)
        tree.write("GPUDatabase.xml",encoding="utf-8",xml_declaration=True)

    def get_node_by_keyvalue(self, nodelist, kv_map):  
        '''''根据属性及属性值定位符合的节点，返回节点 
        nodelist: 节点列表 
        kv_map: 匹配属性及属性值map'''  
        for node in nodelist:  
            if self.if_match(node, kv_map):  
                return node
        return None
    
    def if_match(self, node, kv_map):  
        '''''判断某个节点是否包含所有传入参数属性 
        node: 节点 
        kv_map: 属性及属性值组成的map'''  
        for key in kv_map:  
            if node.get(key) != kv_map.get(key):  
                return False  
        return True


    def BuildQuery(self):
        r=requests.get(self.__url,headers={'user-agent':'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/89.0.4389.90 Safari/537.36'})
        if r.status_code != 200:
            return False
        soup=BeautifulSoup(r.text,'lxml')
        # soup = BeautifulSoup(open(r'C:\Users\wangshuo20\Desktop\Test.html',encoding='utf-8'),'lxml')
        article = soup.select('#page > article')
        ul = article[0].select('#form > section > div > fieldset.filters > ul')
        for li in ul[0].find_all('li'):
            select = li.find('select',attrs={'id':'generation','name':'generation'})
            if select is not None:
                for option in select.find_all('option'):
                    print(option['value'])
                    if len(option['value']) > 0:
                        self.query.append(option['value'])
            Manufacturers = li.find('select',attrs={'id':'mfgr','name':'mfgr'})
            if Manufacturers is not None:
                for option in Manufacturers.find_all('option'):
                    print(option['value'])
                    if len(option['value']) > 0:
                        ET.SubElement(self.__root,"Manufacture",attrib={'name': option['value']})
        return True

    def buildUrl(self,generation):
        url = "{}/?generation={}&sort=released&ajax=1".format(self.__url, quote(generation))
        return url

    def QueryFromGPUDB(self,generation):
        # try:
        #     rd = open(r'C:\Users\wangshuo20\Desktop\{}.json'.format(generation), encoding='utf-8')
        #     return rd
        # except IOError:
        #     return None
        count = 1
        time.sleep(60)
        while count <= 3 :
            url = self.buildUrl(generation)
            r=requests.get(url,headers={'user-agent':'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/89.0.4389.90 Safari/537.36'})
            if r.status_code != 200:
                print('Request: {0} Http Error: {1}'.format(url, r.status_code))
                time.sleep(60*10 * count)
                count += 1
            else:
                return r
        return None

    def __BuildXmlFromData(self, Manufacture):  
        if len(self.data) == 0:
            return
        for gpu in self.data:
            if len(gpu) == 8:
                GPU = ET.SubElement(Manufacture,'GPU',attrib={'name':gpu[0].replace('-','').strip()})

                ChipName = ET.SubElement(GPU,'ChipName')
                ChipName.text = gpu[1].strip()  

                Released = ET.SubElement(GPU,'Released')
                Released.text = gpu[2].strip()

                Bus = ET.SubElement(GPU,'Bus')
                Bus.text = gpu[3].strip()  

                memorySplitStr = gpu[4].split(",")

                if len(memorySplitStr) > 2:
                    MemorySize = ET.SubElement(GPU, "MemorySize")
                    MemorySize.text = memorySplitStr[0].replace(' GB','').strip()

                    MemoryType = ET.SubElement(GPU, "MemoryType")
                    MemoryType.text = memorySplitStr[1].strip()

                    MemoryBits = ET.SubElement(GPU, "MemoryBits")
                    MemoryBits.text = memorySplitStr[2].replace(' bit','').strip()
                
                if gpu[6].find('MHz') != -1:
                    MemoryClock = ET.SubElement(GPU,'MemoryClock')
                    MemoryClock.text = gpu[6].replace(' MHz','').strip() 

                Clock = ET.SubElement(GPU,'Clock')
                Clock.text = gpu[5].replace(' MHz','').strip()

                Others = gpu[7].split(r'/')
                Shaders = ET.SubElement(GPU,'Shaders')
                Shaders.text = Others[0].strip()  

                TMUs = ET.SubElement(GPU,'TMUs')
                TMUs.text = Others[1].strip()  

                ROPs = ET.SubElement(GPU,'ROPs')
                ROPs.text = Others[2].strip()  

    def BuildGPUDBFromQuery(self):
        for generation in self.query:
            print('get {} information'.format(generation))
            r = self.QueryFromGPUDB(generation)
            if r is None:
                continue
            # response = json.loads(r.read())
            response = json.loads(r.text)
            if response['list'] is not None and response['filters'] is not None:
                filters = BeautifulSoup(response['filters'],'lxml')
                ul = filters.select('body > fieldset > ul')
                Manufacture = None
                for li in ul[0].find_all('li'):
                    select = li.find('select',attrs={'id':'mfgr','name':'mfgr'})
                    if select is not None:
                        for option in select.find_all('option'):
                            if len(option['value']) > 0:
                                Manufacture = option['value']
                                break
                if Manufacture is not None:
                    soup = BeautifulSoup(response['list'],'lxml')
                    table = soup.find('table',class_='processors')
                    rows = table.find_all('tr')
                    for row in rows:
                        cols = row.find_all('td')
                        cols = [ele.text.strip() for ele in cols]
                        self.data.append([ele for ele in cols if ele]) # Get rid of empty values
                    self.data = list(filter(None, self.data))
                    node = self.get_node_by_keyvalue(self.__root, {'name': Manufacture})
                    if node is not None:
                        self.__BuildXmlFromData(node)
            self.data = []
            print('Finished get {} information'.format(generation))
    
if __name__ == '__main__':
    resq = GPUDBResquest("https://www.techpowerup.com/gpu-specs")
    if resq.BuildQuery():
        print('gpu Generating')
        resq.BuildGPUDBFromQuery()
        resq.SaveRoot()
        print('gpu Generated')
    else:
        print('error to query gpu info from cpudb')