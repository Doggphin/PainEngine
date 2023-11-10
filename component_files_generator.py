import shutil
import os

def main():
        path = os.path.dirname(os.path.realpath(__file__)).replace('\\', '/')
        inp = input('What should the component be named? ')
        newcpath = path + "/comp_" + inp.lower() + ".c"
        shutil.copyfile(path + "/component_template_c.txt", newcpath)
        with open(newcpath) as f:
                newText=f.read().replace('#COMPONENTNAME#', inp)
                newNewText = newText.replace('#COMPONENTHEADER#', "\"comp_" + inp.lower() + ".h\"")
                fnext = open(newcpath, "w")
                fnext.write(newNewText)
                fnext.close()

        newhpath = path + "/comp_" + inp.lower() + ".h"
        shutil.copyfile(path + "/component_template_h.txt", newhpath)
        with open(newhpath) as f:
                newText=f.read().replace('#COMPONENTNAME#', inp)
                newNewText = newText.replace('#COMPONENTNAMEUPPER#', inp.upper())
                fnext = open(newhpath, "w")
                fnext.write(newNewText)
                fnext.close()

        exit

main()
