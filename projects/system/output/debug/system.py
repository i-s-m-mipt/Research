from html.parser import HTMLParser
from urllib      import request
import json
import numpy as np
import sys

def get_dividends():
    
    class DividendsParser(HTMLParser):
        
        def error(self, message):
            raise ValueError('Unable to parse dividends')

        def __init__(self):
            super().__init__()
            self.inside_table = False
            self.inside_row = False
            self.inside_cell = False
            self.data = []

        def handle_starttag(self, tag, attrs):
            if self.inside_row and tag == 'td':
                self.inside_cell = True
            if self.inside_table and tag == 'tr':
                self.inside_row = True
                self.data.append([])
            if tag == 'table':
                self.inside_table = True

        def handle_data(self, data):
            if self.inside_cell:
                self.data[-1].append(data.strip())

        def handle_endtag(self, tag):
            if tag == 'table':
                self.inside_table = False
            elif tag == 'tr':
                self.inside_row = False
            elif tag == 'td':
                self.inside_cell = False

    url = 'https://smart-lab.ru/dividends/'
    response = request.urlopen(url)
    document = response.read().decode('utf8')

    parser = DividendsParser()
    parser.feed(document)
    parsed = parser.data

    parsed = [{'asset': row[1], 'dividend': float(row[4].replace(',', '.')), 'buy_date': row[7], 'gap_date': row[8]}
              for row in parsed[1:] if len(row) == len(parsed[1])]
    return json.dumps(parsed)

import os

os.environ["CUDA_VISIBLE_DEVICES"] = "-1"
os.environ["TF_CPP_MIN_LOG_LEVEL"] = "3"

import tensorflow as tf

from tensorflow import keras

model_1 = keras.models.load_model("model_21.06.13_5(Dx2d(0.5))Dx2_1.h5", compile = False)
model_2 = keras.models.load_model("model_21.06.13_5(Dx2d(0.5))Dx2_2.h5", compile = False)
model_3 = keras.models.load_model("model_21.06.13_6(Dx2d(0.5))Dx2_1.h5", compile = False)
model_4 = keras.models.load_model("model_21.06.13_7(Dx2d(0.5))Dx2_1.h5", compile = False)

n_models = 4

def predict(asset, scale, data) :

    try:
        
        sample = []
        
        sample.append([float(x) for x in data.split(",")])
        
        sample = np.array(sample).astype(np.float32)

        states = ["L", "S"]

        action = 0

        action += np.argmax(model_1(sample)[0].numpy())
        action += np.argmax(model_2(sample)[0].numpy())
        action += np.argmax(model_3(sample)[0].numpy())
        action += np.argmax(model_4(sample)[0].numpy())

        if (action > n_models / 2):
            action = 1
        else:
            action = 0
        
        return states[action]

    except:

        print("Exception: ", sys.exc_info()[0])

        raise
