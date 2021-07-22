import asyncio
import json
import nltk
import numpy as np
import os
import sys

os.environ["CUDA_VISIBLE_DEVICES"] = "-1"
os.environ["TF_CPP_MIN_LOG_LEVEL"] = "3"

import tensorflow

from datetime    import date, datetime, timedelta
from googletrans import Translator
from html.parser import HTMLParser
from telethon    import TelegramClient
from telethon.tl import types
from tensorflow  import keras
from textblob    import TextBlob
from urllib      import request



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



model_001 = keras.models.load_model("models/model_001.h5", compile = False)
model_011 = keras.models.load_model("models/model_011.h5", compile = False)
model_014 = keras.models.load_model("models/model_014.h5", compile = False)
model_015 = keras.models.load_model("models/model_015.h5", compile = False)
model_019 = keras.models.load_model("models/model_019.h5", compile = False)
model_021 = keras.models.load_model("models/model_021.h5", compile = False)
model_030 = keras.models.load_model("models/model_030.h5", compile = False)
model_032 = keras.models.load_model("models/model_032.h5", compile = False)
model_033 = keras.models.load_model("models/model_033.h5", compile = False)
model_037 = keras.models.load_model("models/model_037.h5", compile = False)
model_042 = keras.models.load_model("models/model_042.h5", compile = False)
model_044 = keras.models.load_model("models/model_044.h5", compile = False)
model_050 = keras.models.load_model("models/model_050.h5", compile = False)
model_053 = keras.models.load_model("models/model_053.h5", compile = False)
model_060 = keras.models.load_model("models/model_060.h5", compile = False)
model_062 = keras.models.load_model("models/model_062.h5", compile = False)
model_063 = keras.models.load_model("models/model_063.h5", compile = False)
model_064 = keras.models.load_model("models/model_064.h5", compile = False)
model_067 = keras.models.load_model("models/model_067.h5", compile = False)
model_068 = keras.models.load_model("models/model_068.h5", compile = False)
model_073 = keras.models.load_model("models/model_073.h5", compile = False)
model_074 = keras.models.load_model("models/model_074.h5", compile = False)
model_081 = keras.models.load_model("models/model_081.h5", compile = False)
model_082 = keras.models.load_model("models/model_082.h5", compile = False)
model_083 = keras.models.load_model("models/model_083.h5", compile = False)
model_084 = keras.models.load_model("models/model_084.h5", compile = False)
model_085 = keras.models.load_model("models/model_085.h5", compile = False)
model_088 = keras.models.load_model("models/model_088.h5", compile = False)
model_089 = keras.models.load_model("models/model_089.h5", compile = False)
model_090 = keras.models.load_model("models/model_090.h5", compile = False)
model_091 = keras.models.load_model("models/model_091.h5", compile = False)
model_092 = keras.models.load_model("models/model_092.h5", compile = False)
model_093 = keras.models.load_model("models/model_093.h5", compile = False)
model_097 = keras.models.load_model("models/model_097.h5", compile = False)
model_099 = keras.models.load_model("models/model_099.h5", compile = False)
model_102 = keras.models.load_model("models/model_102.h5", compile = False)
model_104 = keras.models.load_model("models/model_104.h5", compile = False)
model_105 = keras.models.load_model("models/model_105.h5", compile = False)
model_106 = keras.models.load_model("models/model_106.h5", compile = False)
model_108 = keras.models.load_model("models/model_108.h5", compile = False)
model_109 = keras.models.load_model("models/model_109.h5", compile = False)
model_110 = keras.models.load_model("models/model_110.h5", compile = False)
model_111 = keras.models.load_model("models/model_111.h5", compile = False)
model_112 = keras.models.load_model("models/model_112.h5", compile = False)
model_113 = keras.models.load_model("models/model_113.h5", compile = False)
model_114 = keras.models.load_model("models/model_114.h5", compile = False)
model_115 = keras.models.load_model("models/model_115.h5", compile = False)
model_116 = keras.models.load_model("models/model_116.h5", compile = False)
model_117 = keras.models.load_model("models/model_117.h5", compile = False)
model_118 = keras.models.load_model("models/model_118.h5", compile = False)
model_119 = keras.models.load_model("models/model_119.h5", compile = False)
model_120 = keras.models.load_model("models/model_120.h5", compile = False)
model_121 = keras.models.load_model("models/model_121.h5", compile = False)
model_122 = keras.models.load_model("models/model_122.h5", compile = False)
model_123 = keras.models.load_model("models/model_123.h5", compile = False)
model_124 = keras.models.load_model("models/model_124.h5", compile = False)
model_125 = keras.models.load_model("models/model_125.h5", compile = False)
model_126 = keras.models.load_model("models/model_126.h5", compile = False)
model_127 = keras.models.load_model("models/model_127.h5", compile = False)
model_128 = keras.models.load_model("models/model_128.h5", compile = False)
model_129 = keras.models.load_model("models/model_129.h5", compile = False)
model_130 = keras.models.load_model("models/model_130.h5", compile = False)
model_131 = keras.models.load_model("models/model_131.h5", compile = False)
model_132 = keras.models.load_model("models/model_132.h5", compile = False)

n_models = 64

def predict(asset, scale, data) :

    try:

        sample = []
        
        sample.append([float(x) for x in data.split(",")])
        
        sample = np.array(sample).astype(np.float32)

        states = ["L", "S"]

        s = 0
            
        s += np.argmax(model_001(sample)[0].numpy())
        s += np.argmax(model_011(sample)[0].numpy())
        s += np.argmax(model_014(sample)[0].numpy())
        s += np.argmax(model_015(sample)[0].numpy())
        s += np.argmax(model_019(sample)[0].numpy())
        s += np.argmax(model_021(sample)[0].numpy())
        s += np.argmax(model_030(sample)[0].numpy())
        s += np.argmax(model_032(sample)[0].numpy())
        s += np.argmax(model_033(sample)[0].numpy())
        s += np.argmax(model_037(sample)[0].numpy())
        s += np.argmax(model_042(sample)[0].numpy())
        s += np.argmax(model_044(sample)[0].numpy())
        s += np.argmax(model_050(sample)[0].numpy())
        s += np.argmax(model_053(sample)[0].numpy())
        s += np.argmax(model_060(sample)[0].numpy())
        s += np.argmax(model_062(sample)[0].numpy())
        s += np.argmax(model_063(sample)[0].numpy())
        s += np.argmax(model_064(sample)[0].numpy())
        s += np.argmax(model_067(sample)[0].numpy())
        s += np.argmax(model_068(sample)[0].numpy())
        s += np.argmax(model_073(sample)[0].numpy())
        s += np.argmax(model_074(sample)[0].numpy())
        s += np.argmax(model_081(sample)[0].numpy())
        s += np.argmax(model_082(sample)[0].numpy())
        s += np.argmax(model_083(sample)[0].numpy())
        s += np.argmax(model_084(sample)[0].numpy())
        s += np.argmax(model_085(sample)[0].numpy())
        s += np.argmax(model_088(sample)[0].numpy())
        s += np.argmax(model_089(sample)[0].numpy())
        s += np.argmax(model_090(sample)[0].numpy())
        s += np.argmax(model_091(sample)[0].numpy())
        s += np.argmax(model_092(sample)[0].numpy())
        s += np.argmax(model_093(sample)[0].numpy())
        s += np.argmax(model_097(sample)[0].numpy())
        s += np.argmax(model_099(sample)[0].numpy())
        s += np.argmax(model_102(sample)[0].numpy())
        s += np.argmax(model_104(sample)[0].numpy())
        s += np.argmax(model_105(sample)[0].numpy())
        s += np.argmax(model_106(sample)[0].numpy())
        s += np.argmax(model_108(sample)[0].numpy())
        s += np.argmax(model_109(sample)[0].numpy())
        s += np.argmax(model_110(sample)[0].numpy())
        s += np.argmax(model_111(sample)[0].numpy())
        s += np.argmax(model_112(sample)[0].numpy())
        s += np.argmax(model_113(sample)[0].numpy())
        s += np.argmax(model_114(sample)[0].numpy())
        s += np.argmax(model_115(sample)[0].numpy())
        s += np.argmax(model_116(sample)[0].numpy())
        s += np.argmax(model_117(sample)[0].numpy())
        s += np.argmax(model_118(sample)[0].numpy())
        s += np.argmax(model_119(sample)[0].numpy())
        s += np.argmax(model_120(sample)[0].numpy())
        s += np.argmax(model_121(sample)[0].numpy())
        s += np.argmax(model_122(sample)[0].numpy())
        s += np.argmax(model_123(sample)[0].numpy())
        s += np.argmax(model_124(sample)[0].numpy())
        s += np.argmax(model_125(sample)[0].numpy())
        s += np.argmax(model_126(sample)[0].numpy())
        s += np.argmax(model_127(sample)[0].numpy())
        s += np.argmax(model_128(sample)[0].numpy())
        s += np.argmax(model_129(sample)[0].numpy())
        s += np.argmax(model_130(sample)[0].numpy())
        s += np.argmax(model_131(sample)[0].numpy())
        s += np.argmax(model_132(sample)[0].numpy())

        action = 0

        if (s > n_models / 2):
            action = 1
        else:
            action = 0
        
        return states[action] + " (" + str(max(s, n_models - s)) + " of " + str(n_models) + ")"

    except:

        print("Exception: ", sys.exc_info()[0])

        raise



async def estimate_sentiments_implementation(asset, client):

    filename = "messages.json"

    sentences = []

    if os.path.exists(filename):

        with open(filename, "r", encoding = "utf-8") as fin:
            sentences = json.load(fin)

    else:
    
        fin = open("market/channels.data", "r")

        channels = fin.readlines()
    
        fin.close()
    
        today = date.today()
    
        messages = []
    
        for channel in channels:
        
            channel_data = channel.split(" ")
        
            entity = types.InputPeerChannel(int(channel_data[0]), int(channel_data[1]))
        
            async for message in client.iter_messages(entity, offset_date = today + timedelta(days = 1)):
                if (message.date.year, message.date.month, message.date.day) == (today.year, today.month, today.day):
                    messages.append(message.message)
                else:
                    break
    
        for message in messages:
            try:
                sentences.extend(nltk.tokenize.sent_tokenize(message, language = "russian"))
            except:
                continue

        with open(filename, "w", encoding = "utf-8") as fout:
            json.dump(sentences, fout, ensure_ascii = False, indent = 4)

            
        
    keywords = {
        "BR"  : ["oil", "нефт", "brent"],
        "NG"  : ["gaz", "газ"],
        "GC"  : ["gold", "золот"],
        "LKOH": ["lkoh", "лукойл", "lukoil"],
        "SIBN": ["sibn", "газпромнефт", "gazpromneft"],
        "ROSN": ["rosn", "роснефт", "rosneft"],
        "TATN": ["tatn", "татнефт", "tatneft"],
        "SNGS": ["sngs", "сургутнефтегаз", "surgutneftegas"]
        }
    
    key_sentences = []

    for sentence in sentences:
        for keyword in keywords[asset]:
            if sentence.lower().find(keyword) != -1:
                key_sentences.append(sentence)
                break
                
    translator = Translator()
    
    positive_sentences_counter = 0
    negative_sentences_counter = 0
                
    for key_sentence in key_sentences:
        
        try:
            translated_sentence = translator.translate(key_sentence, src = "ru", dest = "en").text
        except:
            continue
            
        polarity = TextBlob(translated_sentence).sentiment.polarity
        
        if polarity > 0.0:
            positive_sentences_counter += 1
            
        if polarity < 0.0:
            negative_sentences_counter += 1

    s1 = "%2d" % positive_sentences_counter
    s2 = "%2d" % negative_sentences_counter
        
    return s1 + "(P), " + s2 + "(N)"

def estimate_sentiments(asset, username, api_id, api_hash):

    client = TelegramClient(username, api_id, api_hash)
    
    client.start()

    with client:
        result = client.loop.run_until_complete(estimate_sentiments_implementation(asset, client))

    return result
