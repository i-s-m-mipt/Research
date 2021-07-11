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



model_01 = keras.models.load_model("models/model_01.h5", compile = False)
model_02 = keras.models.load_model("models/model_02.h5", compile = False)
model_03 = keras.models.load_model("models/model_03.h5", compile = False)
model_04 = keras.models.load_model("models/model_04.h5", compile = False)
model_05 = keras.models.load_model("models/model_05.h5", compile = False)
model_06 = keras.models.load_model("models/model_06.h5", compile = False)
model_07 = keras.models.load_model("models/model_07.h5", compile = False)
model_08 = keras.models.load_model("models/model_08.h5", compile = False)
model_09 = keras.models.load_model("models/model_09.h5", compile = False)
model_10 = keras.models.load_model("models/model_10.h5", compile = False)
model_11 = keras.models.load_model("models/model_11.h5", compile = False)
model_12 = keras.models.load_model("models/model_12.h5", compile = False)
model_13 = keras.models.load_model("models/model_13.h5", compile = False)
model_14 = keras.models.load_model("models/model_14.h5", compile = False)
model_15 = keras.models.load_model("models/model_15.h5", compile = False)
model_16 = keras.models.load_model("models/model_16.h5", compile = False)
model_17 = keras.models.load_model("models/model_17.h5", compile = False)
model_18 = keras.models.load_model("models/model_18.h5", compile = False)
model_19 = keras.models.load_model("models/model_19.h5", compile = False)
model_20 = keras.models.load_model("models/model_20.h5", compile = False)
model_21 = keras.models.load_model("models/model_21.h5", compile = False)
model_22 = keras.models.load_model("models/model_22.h5", compile = False)
model_23 = keras.models.load_model("models/model_23.h5", compile = False)
model_24 = keras.models.load_model("models/model_24.h5", compile = False)
model_25 = keras.models.load_model("models/model_25.h5", compile = False)
model_26 = keras.models.load_model("models/model_26.h5", compile = False)
model_27 = keras.models.load_model("models/model_27.h5", compile = False)
model_28 = keras.models.load_model("models/model_28.h5", compile = False)
model_29 = keras.models.load_model("models/model_29.h5", compile = False)
model_30 = keras.models.load_model("models/model_30.h5", compile = False)

def predict(asset, scale, data) :

    try:

        n_models = 30

        sample = []
        
        sample.append([float(x) for x in data.split(",")])
        
        sample = np.array(sample).astype(np.float32)

        states = ["L", "S"]

        s = 0
            
        s += np.argmax(model_02(sample)[0].numpy())
        s += np.argmax(model_03(sample)[0].numpy())
        s += np.argmax(model_05(sample)[0].numpy())
        s += np.argmax(model_07(sample)[0].numpy())
        s += np.argmax(model_09(sample)[0].numpy())
        s += np.argmax(model_10(sample)[0].numpy())
        s += np.argmax(model_14(sample)[0].numpy())
        s += np.argmax(model_16(sample)[0].numpy())
        s += np.argmax(model_17(sample)[0].numpy())
        s += np.argmax(model_18(sample)[0].numpy())
        s += np.argmax(model_19(sample)[0].numpy())
        s += np.argmax(model_20(sample)[0].numpy())
        s += np.argmax(model_21(sample)[0].numpy())
        s += np.argmax(model_22(sample)[0].numpy())
        s += np.argmax(model_24(sample)[0].numpy())
        s += np.argmax(model_26(sample)[0].numpy())
        s += np.argmax(model_27(sample)[0].numpy())
        s += np.argmax(model_28(sample)[0].numpy())
        s += np.argmax(model_29(sample)[0].numpy())
        s += np.argmax(model_30(sample)[0].numpy())

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
                
    translator = Translator()
    
    positive_sentences_counter = 0
    negative_sentences_counter = 0
                
    for key_sentence in key_sentences:
        
        polarity = TextBlob(translator.translate(key_sentence, src = "ru", dest = "en").text).sentiment.polarity
        
        if polarity > 0.0:
            positive_sentences_counter += 1
            
        if polarity < 0.0:
            negative_sentences_counter += 1
        
    return str(positive_sentences_counter) + "(P), " + str(negative_sentences_counter) + "(N)"

def estimate_sentiments(asset, username, api_id, api_hash):

    return asyncio.run(estimate_sentiments_implementation(username, api_id, api_hash))
