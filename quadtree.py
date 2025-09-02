class QuadNode:
    def __init__(self, value=None, children=None, parent=None, sontype=0, coords=None,pathparent=None,cordinates=None):
        self.value = value              # white or black
        self.children = children        # list of 4 children
        self.parent = parent            # parent node
        self.sontype = sontype          # index: 0-NW, 1-NE, 2-SW, 3-SE
        self.coords = coords  
        self.cordinates=cordinates
        self.pathparent=pathparent      
    def is_leaf(self):
        return self.children is None

    def __repr__(self, level=0):
        indent = "  " * level
        coord_str = f" {self.coords}" if self.coords else ""
        if self.is_leaf():
            return f"{indent}Leaf({self.value}){coord_str}"
        else:
            s = f"{indent}Node:{coord_str}\n"
            directions = ['NW', 'NE', 'SW', 'SE']
            for dir, child in zip(directions, self.children):
                s += f"{indent}  {dir}:\n{child.__repr__(level + 2)}\n"
            return s

def is_uniform(matrix):
    first = matrix[0][0]
    return all(cell == first for row in matrix for cell in row)


def build_quadtree(matrix, parent=None, sontype=0, r_start=0, c_start=0):
    size = len(matrix)
    cordinates = ((r_start+r_start + size - 1)//2,(c_start+c_start + size - 1)//2)
    coords=(r_start,r_start+size-1,c_start,c_start+size-1)
    if is_uniform(matrix):
        return QuadNode(value=matrix[0][0], parent=parent, sontype=sontype, coords=coords,cordinates=cordinates)

    half = size // 2

    def subregion(matrix, row_start, col_start, size):
        return [row[col_start:col_start + size] for row in matrix[row_start:row_start + size]]


    half = size // 2
    node = QuadNode(parent=parent, sontype=sontype, coords=coords)
    node.children = [
    build_quadtree(subregion(matrix, 0, 0, half), node, 0, r_start, c_start),  # NW
    build_quadtree(subregion(matrix, 0, half, half), node, 1, r_start, c_start + half),  # NE
    build_quadtree(subregion(matrix, half, 0, half), node, 2, r_start + half, c_start),  # SW
    build_quadtree(subregion(matrix, half, half, half), node, 3, r_start + half, c_start + half)  # SE
    ]

    return node