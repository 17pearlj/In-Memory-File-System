
class FileSystem {
    public:
    class node {
        public:
            string name;
            bool isDir;
            node *parent; 

            node(string name, bool isDir, node *parent) {
                this->isDir =isDir;
                this->parent = parent;
                this->name = name;
            }
    };
    class fileNode : public node {
        public:
        string content;
        
        void appendContent(string contentToAppend) {
            this->content.append(contentToAppend);
        }
        fileNode(string name, bool isDir, node *parent, string content) : node(name, isDir, parent) {
            this->content = content;
        }
    };
    class dirNode : public node {
        unordered_map<string, node*>children;
        public:
        void getChildren(vector<string>& v) {
            for(unordered_map<string, node*>::iterator it = children.begin(); it != children.end(); ++it) {
                v.push_back(it->first);
            }
            sort(v.begin(), v.end());
        }

        node* getOrAddChild(string s, bool isDir) {
            return getOrAddChild(s, isDir, "");
        }
        node* getOrAddChild(string s, bool isDir, string content) {
            if (children.find(s) == children.end()) { 
                node *child;
                if (isDir) {
                    child = new dirNode(s, isDir, this);
                } else {
                    child = new fileNode(s, isDir, this, content);
                }
                children[s] = child;
                return child;
            } else {
                if (!isDir) {                  
                    ((fileNode *)children[s])->appendContent(content);
                }
                return children[s];
            }
        }
        node * getChild(string s) {
            if (children.find(s) != children.end()) { 
                return children[s];
            }
            return nullptr;
        }

        dirNode(string name, bool isDir, node *parent) : node(name, isDir, parent)   {            
        }
        dirNode(string name, bool isDir, node *parent, unordered_map<string, node*>children) : node(name, isDir, parent)   {
            this->children = children; 
        }
    };


    dirNode* root;


    vector<string> getPath(string path) {
        string build = "";
        vector<string> v;
        for (char c : path) {
            if (c == '/'){
                if (build != "") {v.push_back(build); }
                
                build = "";
            } else {
                build += c;
            }
        }
        if (build != "") {v.push_back(build); }
        return v;
    }

    FileSystem() {

        unordered_map<string, node*>children;

        root = new dirNode("", true, nullptr, children);

    }
    
    vector<string> ls(string path) {
        vector<string> v = getPath(path);
        node *parent = root;


        for (int i = 0; i < v.size(); i++) {
           parent = ((dirNode*)parent)->getChild(v[i]);
        }
        vector<string> sol;
        if (parent) {
            if (parent->isDir) {
                ((dirNode*)parent)->getChildren(sol);
            } else {
                sol.push_back(parent->name);
            }
        }

        return sol;
    }
    
    void mkdir(string path) {
        vector<string> v = getPath(path);

        node *parent = root;

        for (int i = 0; i < v.size(); i++) {
            parent = ((dirNode*)parent)->getOrAddChild(v[i], true);
        }
        
    }
    
    void addContentToFile(string filePath, string content) {
        vector<string> v = getPath(filePath);
        string c = "";
        node *parent = root;

        for (int i = 0; i < v.size(); i++) {
            if (i == v.size() - 1) { c = content; }
            parent = ((dirNode*)parent)->getOrAddChild(v[i], (i != v.size() - 1), c);
        }
    }
    
    string readContentFromFile(string filePath) {
        vector<string> v = getPath(filePath);

        node *parent = root;
        
        for (int i = 0; i < v.size(); i++) {
            parent = ((dirNode*)parent)->getChild(v[i]);
            if (!parent) { return ""; }
        }
        return ((fileNode*)parent)->content;
    }
};

/**
 * Your FileSystem object will be instantiated and called as such:
 * FileSystem* obj = new FileSystem();
 * vector<string> param_1 = obj->ls(path);
 * obj->mkdir(path);
 * obj->addContentToFile(filePath,content);
 * string param_4 = obj->readContentFromFile(filePath);
 */
