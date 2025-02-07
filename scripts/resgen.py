import os.path
import os
import sys

from lxml import etree


print(os.path.dirname(sys.argv[0]))

os.chdir(os.path.dirname(sys.argv[0]))


def DoMain():

    print("执行...\n")

    root: etree._Element = etree.Element("resource")
    for x in os.listdir('./'):
        if os.path.isdir(x) and x != '.idea':
            p: etree._Element = etree.SubElement(root, x)
            p.set('folder',x)
            p.set('id',x.upper())

            for y in os.listdir(x):
                if os.path.isfile(f'{x}/{y}'):
                    ele: etree._Element = etree.SubElement(p,'Element')
                    ele.set('path',y)
                    ele.set('id',(x + "_" + y.split('.')[-2]).upper())
                    print(y)
    # p:etree._Element = etree.SubElement(root,'images')
    # p.set('folder','images')
    # p.set('id','IMAGE')

    f = open('ResourceList.xml', 'w+', encoding='utf-8')
    ot = etree.tostring(root, pretty_print=True)
    f.write(ot.decode('utf-8'))

    f.close()
    print(ot.decode('utf-8'))



if __name__ == "__main__":
    DoMain()

