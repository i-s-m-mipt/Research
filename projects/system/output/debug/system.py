from html.parser import HTMLParser
from urllib      import request
import json
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
            self.crutch = ""
            self.data = []

        def handle_starttag(self, tag, attrs):
            if self.inside_cell and tag == 'br':
                self.crutch = self.data[-1][-1]
                self.data[-1].pop(-1)
            if self.inside_row and tag in ['td', 'th']:
                self.inside_cell = True
            if self.inside_table and tag == 'tr':
                self.inside_row = True
                self.data.append([])
            if tag == 'table':
                self.inside_table = True

        def handle_data(self, data):
            if self.inside_cell:
                self.data[-1].append(self.crutch + data.strip())
                self.crutch = ""

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

    parsed[0] = [x for x in parsed[0][::2] if len(x) > 0]
    parsed[0].insert(3, '')
    parsed[1] = parsed[1][1:]

    parsed = [{'asset': row[1], 'dividend': float(row[4].replace(',', '.')), 'buy_date': row[7], 'gap_date': row[8]}
              for row in parsed[1:] if len(row) == len(parsed[0])]
    return json.dumps(parsed)

# load trained models here as globals

def predict(asset, scale, data) :

    try:

        # call predict of required model for asset, scale here

        # use data as required prehistory for predict function
        
        return "C" # or "L", "S"

    except:

        print("Exception: ", sys.exc_info()[0])

        raise
