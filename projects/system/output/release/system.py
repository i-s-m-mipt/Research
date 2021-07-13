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
model_009 = keras.models.load_model("models/model_009.h5", compile = False)
model_011 = keras.models.load_model("models/model_011.h5", compile = False)
model_014 = keras.models.load_model("models/model_014.h5", compile = False)
model_015 = keras.models.load_model("models/model_015.h5", compile = False)
model_019 = keras.models.load_model("models/model_019.h5", compile = False)
model_021 = keras.models.load_model("models/model_021.h5", compile = False)
model_024 = keras.models.load_model("models/model_024.h5", compile = False)
model_026 = keras.models.load_model("models/model_026.h5", compile = False)
model_030 = keras.models.load_model("models/model_030.h5", compile = False)
model_032 = keras.models.load_model("models/model_032.h5", compile = False)
model_033 = keras.models.load_model("models/model_033.h5", compile = False)
model_036 = keras.models.load_model("models/model_036.h5", compile = False)
model_037 = keras.models.load_model("models/model_037.h5", compile = False)
model_040 = keras.models.load_model("models/model_040.h5", compile = False)
model_041 = keras.models.load_model("models/model_041.h5", compile = False)
model_042 = keras.models.load_model("models/model_042.h5", compile = False)
model_044 = keras.models.load_model("models/model_044.h5", compile = False)
model_049 = keras.models.load_model("models/model_049.h5", compile = False)
model_050 = keras.models.load_model("models/model_050.h5", compile = False)
model_053 = keras.models.load_model("models/model_053.h5", compile = False)
model_054 = keras.models.load_model("models/model_054.h5", compile = False)
model_060 = keras.models.load_model("models/model_060.h5", compile = False)
model_062 = keras.models.load_model("models/model_062.h5", compile = False)
model_063 = keras.models.load_model("models/model_063.h5", compile = False)
model_064 = keras.models.load_model("models/model_064.h5", compile = False)
model_066 = keras.models.load_model("models/model_066.h5", compile = False)
model_067 = keras.models.load_model("models/model_067.h5", compile = False)
model_068 = keras.models.load_model("models/model_068.h5", compile = False)
model_069 = keras.models.load_model("models/model_069.h5", compile = False)
model_073 = keras.models.load_model("models/model_073.h5", compile = False)
model_074 = keras.models.load_model("models/model_074.h5", compile = False)
model_075 = keras.models.load_model("models/model_075.h5", compile = False)
model_076 = keras.models.load_model("models/model_076.h5", compile = False)
model_077 = keras.models.load_model("models/model_077.h5", compile = False)
model_078 = keras.models.load_model("models/model_078.h5", compile = False)
model_079 = keras.models.load_model("models/model_079.h5", compile = False)
model_080 = keras.models.load_model("models/model_080.h5", compile = False)
model_081 = keras.models.load_model("models/model_081.h5", compile = False)
model_082 = keras.models.load_model("models/model_082.h5", compile = False)
model_083 = keras.models.load_model("models/model_083.h5", compile = False)
model_084 = keras.models.load_model("models/model_084.h5", compile = False)
model_085 = keras.models.load_model("models/model_085.h5", compile = False)
model_086 = keras.models.load_model("models/model_086.h5", compile = False)
model_087 = keras.models.load_model("models/model_087.h5", compile = False)
model_088 = keras.models.load_model("models/model_088.h5", compile = False)
model_089 = keras.models.load_model("models/model_089.h5", compile = False)
model_090 = keras.models.load_model("models/model_090.h5", compile = False)
model_091 = keras.models.load_model("models/model_091.h5", compile = False)
model_092 = keras.models.load_model("models/model_092.h5", compile = False)
model_093 = keras.models.load_model("models/model_093.h5", compile = False)
model_094 = keras.models.load_model("models/model_094.h5", compile = False)
model_095 = keras.models.load_model("models/model_095.h5", compile = False)
model_097 = keras.models.load_model("models/model_097.h5", compile = False)
model_098 = keras.models.load_model("models/model_098.h5", compile = False)
model_099 = keras.models.load_model("models/model_099.h5", compile = False)
model_100 = keras.models.load_model("models/model_100.h5", compile = False)
model_101 = keras.models.load_model("models/model_101.h5", compile = False)
model_102 = keras.models.load_model("models/model_102.h5", compile = False)
model_103 = keras.models.load_model("models/model_103.h5", compile = False)
model_104 = keras.models.load_model("models/model_104.h5", compile = False)
model_105 = keras.models.load_model("models/model_105.h5", compile = False)
model_106 = keras.models.load_model("models/model_106.h5", compile = False)
model_107 = keras.models.load_model("models/model_107.h5", compile = False)

n_models = 64

def predict(asset, scale, data) :

    try:

        sample = []
        
        sample.append([float(x) for x in data.split(",")])
        
        sample = np.array(sample).astype(np.float32)

        states = ["L", "S"]

        s = 0
            
        s += np.argmax(model_001(sample)[0].numpy())
        s += np.argmax(model_009(sample)[0].numpy())
        s += np.argmax(model_011(sample)[0].numpy())
        s += np.argmax(model_014(sample)[0].numpy())
        s += np.argmax(model_015(sample)[0].numpy())
        s += np.argmax(model_019(sample)[0].numpy())
        s += np.argmax(model_021(sample)[0].numpy())
        s += np.argmax(model_024(sample)[0].numpy())
        s += np.argmax(model_026(sample)[0].numpy())
        s += np.argmax(model_030(sample)[0].numpy())
        s += np.argmax(model_032(sample)[0].numpy())
        s += np.argmax(model_033(sample)[0].numpy())
        s += np.argmax(model_036(sample)[0].numpy())
        s += np.argmax(model_037(sample)[0].numpy())
        s += np.argmax(model_040(sample)[0].numpy())
        s += np.argmax(model_041(sample)[0].numpy())
        s += np.argmax(model_042(sample)[0].numpy())
        s += np.argmax(model_044(sample)[0].numpy())
        s += np.argmax(model_049(sample)[0].numpy())
        s += np.argmax(model_050(sample)[0].numpy())
        s += np.argmax(model_053(sample)[0].numpy())
        s += np.argmax(model_054(sample)[0].numpy())
        s += np.argmax(model_060(sample)[0].numpy())
        s += np.argmax(model_062(sample)[0].numpy())
        s += np.argmax(model_063(sample)[0].numpy())
        s += np.argmax(model_064(sample)[0].numpy())
        s += np.argmax(model_066(sample)[0].numpy())
        s += np.argmax(model_067(sample)[0].numpy())
        s += np.argmax(model_068(sample)[0].numpy())
        s += np.argmax(model_069(sample)[0].numpy())
        s += np.argmax(model_073(sample)[0].numpy())
        s += np.argmax(model_074(sample)[0].numpy())
        s += np.argmax(model_075(sample)[0].numpy())
        s += np.argmax(model_076(sample)[0].numpy())
        s += np.argmax(model_077(sample)[0].numpy())
        s += np.argmax(model_078(sample)[0].numpy())
        s += np.argmax(model_079(sample)[0].numpy())
        s += np.argmax(model_080(sample)[0].numpy())
        s += np.argmax(model_081(sample)[0].numpy())
        s += np.argmax(model_082(sample)[0].numpy())
        s += np.argmax(model_083(sample)[0].numpy())
        s += np.argmax(model_084(sample)[0].numpy())
        s += np.argmax(model_085(sample)[0].numpy())
        s += np.argmax(model_086(sample)[0].numpy())
        s += np.argmax(model_087(sample)[0].numpy())
        s += np.argmax(model_088(sample)[0].numpy())
        s += np.argmax(model_089(sample)[0].numpy())
        s += np.argmax(model_090(sample)[0].numpy())
        s += np.argmax(model_091(sample)[0].numpy())
        s += np.argmax(model_092(sample)[0].numpy())
        s += np.argmax(model_093(sample)[0].numpy())
        s += np.argmax(model_094(sample)[0].numpy())
        s += np.argmax(model_095(sample)[0].numpy())
        s += np.argmax(model_097(sample)[0].numpy())
        s += np.argmax(model_098(sample)[0].numpy())
        s += np.argmax(model_099(sample)[0].numpy())
        s += np.argmax(model_100(sample)[0].numpy())
        s += np.argmax(model_101(sample)[0].numpy())
        s += np.argmax(model_102(sample)[0].numpy())
        s += np.argmax(model_103(sample)[0].numpy())
        s += np.argmax(model_104(sample)[0].numpy())
        s += np.argmax(model_105(sample)[0].numpy())
        s += np.argmax(model_106(sample)[0].numpy())
        s += np.argmax(model_107(sample)[0].numpy())

        action = 0

        if (s > n_models / 2):
            action = 1
        else:
            action = 0
        
        return states[action] + " (" + str(max(s, n_models - s)) + " of " + str(n_models) + ")"

    except:

        print("Exception: ", sys.exc_info()[0])

        raise



async def estimate_sentiments_implementation(username, api_id, api_hash):
    
    client = TelegramClient(username, api_id, api_hash)
    
    await client.start()
    
    filename = "market/channels.data"
    
    fin = open(filename, "r")

    channels = fin.readlines()
    
    fin.close()
    
    today = date.today()
    
    messages = []
    
    for channel in channels:
        
        channel_data = channel.split(" ")
        
        channel_id   = int(channel_data[0])
        channel_hash = int(channel_data[1])
        
        entity = types.InputPeerChannel(channel_id, channel_hash)
        
        async for message in client.iter_messages(entity, offset_date = today + timedelta(days = 1)):
            if (message.date.year, message.date.month, message.date.day) == (today.year, today.month, today.day):
                messages.append(message.message)
            else:
                break
    
    sentences = []
    
    for message in messages:
        try:
            sentences.extend(nltk.tokenize.sent_tokenize(message, language = "russian"))
        except:
            continue
        
    keywords = ["lkoh", "лукойл", "lukoil", "brent", "wti", "urals", "oil", "crude", "нефть"]
    
    key_sentences = []

    for sentence in sentences:
        for keyword in keywords:
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
        
    return str(positive_sentences_counter) + "(P), " + str(negative_sentences_counter) + "(N)"

def estimate_sentiments(asset, username, api_id, api_hash):

    return asyncio.run(estimate_sentiments_implementation(username, api_id, api_hash))
