# app.py
from flask import Flask, request, jsonify, render_template
import ctypes
import os

app = Flask(__name__, template_folder=".")

# === AUTO LOAD YOUR COMPILED LIBRARY ===
import platform
lib_name = "libcampus.so" if platform.system() != "Windows" else "libcampus.dll"

try:
    lib = ctypes.CDLL(f"./{lib_name}")
    print(f"Successfully loaded {lib_name}")
except Exception as e:
    raise Exception(f"Cannot load {lib_name} — did you compile it? Error: {e}")

# === C STRUCTURES ===
class Node(ctypes.Structure):
    _fields_ = [("vertex", ctypes.c_int), ("cost", ctypes.c_int),
                ("distance", ctypes.c_int), ("next", ctypes.c_void_p)]

class Graph(ctypes.Structure):
    _fields_ = [("V", ctypes.c_int),
                ("adjList", ctypes.POINTER(ctypes.c_void_p)),
                ("names", (ctypes.c_char * 50) * 100)]

class EdgeMST(ctypes.Structure):
    _fields_ = [("src", ctypes.c_int), ("dest", ctypes.c_int),
                ("cost", ctypes.c_int), ("distance", ctypes.c_int)]

# === TELL PYTHON THE FUNCTION SIGNATURES ===

# Graph*, int -> Graph*
lib.createGraph.restype = ctypes.POINTER(Graph)
lib.createGraph.argtypes = [ctypes.c_int]

# void addEdge(Graph* g, int src, int dest, int cost, int distance);
lib.addEdge.restype = None
lib.addEdge.argtypes = [
    ctypes.POINTER(Graph),
    ctypes.c_int,  # src
    ctypes.c_int,  # dest
    ctypes.c_int,  # cost
    ctypes.c_int   # distance
]

# bool isConnected(Graph* graph, int src, int dest);
lib.isConnected.restype = ctypes.c_bool
lib.isConnected.argtypes = [
    ctypes.POINTER(Graph),
    ctypes.c_int,  # src
    ctypes.c_int   # dest
]

# int dijkstraDistance(Graph* graph, int src, int dest);
lib.dijkstraDistance.restype = ctypes.c_int
lib.dijkstraDistance.argtypes = [ctypes.POINTER(Graph),
                                 ctypes.c_int,
                                 ctypes.c_int]


# Optional, only if you decide to call them from Python later:

# Node* createNode(int v, int cost, int distance);
lib.createNode.restype = ctypes.POINTER(Node)
lib.createNode.argtypes = [
    ctypes.c_int,  # vertex
    ctypes.c_int,  # cost
    ctypes.c_int   # distance
]

# void printGraph(Graph* g);
lib.printGraph.restype = None
lib.printGraph.argtypes = [ctypes.POINTER(Graph)]

# MST related
lib.computeMST.restype = ctypes.c_int
lib.computeMST.argtypes = [
    ctypes.POINTER(Graph),
    ctypes.POINTER(EdgeMST),
    ctypes.c_int
]

# === ROUTES ===

@app.route("/")
def index():
    # Serves GUI.html from the current folder
    return render_template("GUI.html")


@app.route("/analyze", methods=["POST"])
def analyze():
    data = request.get_json()

    # Expecting something like:
    # {
    #   "buildings": ["A", "B", "C", ...],
    #   "edges": [
    #       {"from": 0, "to": 1, "cost": 10, "dist": 50},
    #       ...
    #   ],
    #   "src": 0,
    #   "dest": 3
    # }

    buildings = data["buildings"]
    edges = data["edges"]
    src = int(data["src"])
    dest = int(data["dest"])

    n = len(buildings)

    # Create graph
    g = lib.createGraph(n)

    # Set names in Python-side list only; C uses indices
    # Add edges to C graph
    for e in edges:
        lib.addEdge(
            g,
            int(e["from"]),
            int(e["to"]),
            int(e["cost"]),
            int(e["dist"]),
        )

    # Connectivity
    connected = bool(lib.isConnected(g, src, dest))

    # Shortest distance (using your new dijkstraDistance wrapper)
    shortest = lib.dijkstraDistance(g, src, dest)
    if shortest == -1:
        shortest = None

    # MST: allocate an array of EdgeMST and call computeMST
    MAX_EDGES = 256
    MSTArrayType = EdgeMST * MAX_EDGES
    mst_array = MSTArrayType()

    mst_cost = lib.computeMST(g, mst_array, MAX_EDGES)

    mst_edges = []
    for e in mst_array:
        # Treat fully zero struct as "no more valid edges"
        if e.src == 0 and e.dest == 0 and e.cost == 0 and e.distance == 0:
            continue  # or break if you guarantee all real edges come first

        if 0 <= e.src < n and 0 <= e.dest < n:
            mst_edges.append({
                "from": buildings[e.src],
                "to": buildings[e.dest],
                "cost": e.cost,
                "dist": e.distance,
            })

    result = {
        "connected": connected,
        "shortest_distance": shortest,
        "mst_cost": mst_cost,
        "mst_edges": mst_edges,
    }
    return jsonify(result)


# === RUN HTTPS SERVER ===
if __name__ == "__main__":
    # Generate SSL cert if not exists
    if not os.path.exists("ssl"):
        os.mkdir("ssl")
        os.system('openssl req -x509 -newkey rsa:4096 -nodes -out ssl/cert.pem -keyout ssl/key.pem -days 3650 -subj "/CN=localhost"')
    
    print("Starting HTTPS server at https://localhost:5000")
    app.run(host="0.0.0.0", port=5000, ssl_context=('ssl/cert.pem', 'ssl/key.pem'))