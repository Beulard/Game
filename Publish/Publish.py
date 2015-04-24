import os
import os.path
import shutil

if os.path.exists('pb'):
    shutil.rmtree('pb')

os.mkdir('pb')
os.mkdir('pb/data')

print('\ncopying exe and config...')
for root, _, files in os.walk('bin/'):
    for f in files:
        ext = f.split('.')
        if ext[1] == 'exe' or ext[1] == 'json':
            print('  copying \'' + f + '\'')
            shutil.copy('bin/' + f, 'pb/')

print('\ncopying data folder...')
for root, _, files in os.walk('data/'):
    for f in files:
        print('  copying \'' + f + '\'')
        shutil.copy('data/' + f, 'pb/data')

print('\nall done.')
