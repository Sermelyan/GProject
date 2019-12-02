from flask import Flask
from flask import request, jsonify

app = Flask(__name__)

def getDistance(xA, yA, xB, yB):
    answer = int((   ( ( float(xA) - float(xB) )**2 + ( float(yA) - float(yB) )**2 )**0.5  ))
    return answer  # * рандомный коэффициент пробки


def getWeights(coordinatesList):
    weightDict = {}
    print("COOR:"+str(coordinatesList) )
    points = coordinatesList.keys()
    print("POINTS:" + str(points))
    
    for i in range(len(coordinatesList)):
        for j in range(len(coordinatesList)):
            if i == j:
                continue
            #print("KEY: "+str(i)+"->"+str(j))
            weightDict[ str(i)+"->"+str(j) ] = getDistance(coordinatesList.get(str(i)).get("X"),
                                                         coordinatesList.get(str(i)).get("Y"),
                                                         coordinatesList.get(str(j)).get("X"),
                                                         coordinatesList.get(str(j)).get("Y") )
    ribsWeight = {}
    ribsWeight["weights"] = weightDict
    ribsWeight = jsonify(ribsWeight)
    return ribsWeight


@app.route('/api', methods=['GET', 'POST'])
def index():
    if request.method == "POST":
        #print("=======HEADERS======\n" + str(request.headers) + "\n==== END HEADERS ===\n")
        json_data = request.get_json()
        #print(json_data)
        coordinatesList = json_data["coordinates"]
        #print("coor:"+str(coordinatesList))
        answer = getWeights(coordinatesList)
        #print("\n\nANSWER:"+ str(answer))
        return answer



if __name__ == "__main__":
    app.run()
