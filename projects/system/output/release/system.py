import sys

# load trained models here as globals

def predict(asset, scale, data) :

    try:

        # call predict of required model for asset, scale here

        # use data as required prehistory for predict function
        
        return "WW" # or "OL", "CL", "OS", "CS"

    except:

        print("Exception: ", sys.exc_info()[0])

        raise
