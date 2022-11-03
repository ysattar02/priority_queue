// CMSC 341 - Spring 2022 - Project 3
#include "squeue.h"
SQueue::SQueue(prifn_t priFn, HEAPTYPE heapType)
        :m_heap(nullptr), m_size(0), m_priorFunc(priFn), m_heapType(heapType)
{
    //done
}

SQueue::~SQueue(){
    //done
    clear();
}

SQueue::SQueue(const SQueue& rhs)
        :m_heap(nullptr), m_size(rhs.m_size), m_priorFunc(rhs.m_priorFunc), m_heapType(rhs.m_heapType)
{
    //done
    m_heap = r_deepCopy(rhs.m_heap);
    m_size = rhs.m_size;
}

SQueue& SQueue::operator=(const SQueue& rhs){
    //done
    if (this != &rhs){ //checking for self assignment
        clear();
        this->m_heap = r_deepCopy(rhs.m_heap);
    }
    return *this;
}

void SQueue::insertPost(const Post& post){
    //done
    SQueue temp(m_priorFunc, m_heapType);
    temp.m_heap = new Node(post); //temp queue w one post in it
    temp.m_size = 1;
    mergeWithQueue(temp); //most of insert process is done in merge function
}

Post SQueue::getNextPost(){
    //done
    if (m_size == 0){
        throw out_of_range("Empty Heap, Cannot call Function.");
    }
    Node* pret = m_heap; //temp ptr
    Node* pright = m_heap->m_right;
    m_heap = m_heap->m_left; //point to left child
    pret->m_left = nullptr;
    pret->m_right = nullptr;
    m_heap = r_merge(m_heap, pright);
    m_size--;
    Post temp(pret->m_post);
    delete pret;
    pret = nullptr;
    pright = nullptr;
    return temp;
}

void SQueue::mergeWithQueue(SQueue& rhs){
    //done
    if (this->m_priorFunc != rhs.m_priorFunc){
        throw domain_error("Merge with Queue called with mismatched priority functions.");
    }
    if (this != &rhs){ //check for self assignment, this executes if not same
        this->m_heap = r_merge(this->m_heap, rhs.m_heap);
        this->m_size += rhs.m_size;
        rhs.m_heap = nullptr;
        rhs.m_size = 0;
    }
}

void SQueue::clear() {
    //done
    r_clear(m_heap);
}

int SQueue::numPosts() const{
    //done
    return m_size;
}

void SQueue::printPostsQueue() const{
    //done
    r_print(m_heap);
}

prifn_t SQueue::getPriorityFn() const{
    //done
    return m_priorFunc;
}

void SQueue::setPriorityFn(prifn_t priFn, HEAPTYPE heapType) {
    //done
    m_priorFunc = priFn; //setter
    m_heapType = heapType; //setter
    Node* o_heap = m_heap;
    m_heap = nullptr;
    r_rebuild(o_heap);
}

void SQueue::dump() const
{
    // To view a graphical representation of the dump output, follow the link below:
    // https://www.csee.umbc.edu/~donyaee/341/res/tree_viewer.html
    if (m_size == 0) {
        cout << "Empty skew heap.\n" ;
    } else {
        dump(m_heap);
        cout << endl;
    }
}

void SQueue::dump(Node *pos) const {
    if ( pos != nullptr ) {
        cout << "(";
        dump(pos->m_left);
        cout << m_priorFunc(pos->m_post) << ":" << pos->m_post.getPostID();
        dump(pos->m_right);
        cout << ")";
    }
}

ostream& operator<<(ostream& sout, const Post& post) {
    sout << "Post ID: " << post.getPostID()
         << ", number of likes: " << post.getNumLikes()
         << ", interest level: " << post.getInterestLevel()
         << ", connection level: " << post.getConnectLevel()
         << ", post time: " << post.getPostTime();
    return sout;
}

ostream& operator<<(ostream& sout, const Node& node) {
    sout << node.getPost();
    return sout;
}

void SQueue::r_clear(Node* temp){
    //done
    if (temp == nullptr){
        return;
    }
    r_clear(temp->m_left);
    r_clear(temp->m_right);
    temp->m_left = nullptr;
    temp->m_right = nullptr;
    delete temp;
    temp = nullptr;
}

Node* SQueue::r_deepCopy(const Node *src) {
    //done
    if (src == nullptr){ //base case
        return nullptr;
    }
    Node* dest = new Node(src->getPost());
    dest->m_left = r_deepCopy(src->m_left);
    dest->m_right = r_deepCopy(src->m_right);
    return dest;
}

Node* SQueue::r_merge(Node *p1, Node *p2) {
    //done
    if (p1 == nullptr){
        return p2;
    }
    if (p2 == nullptr){
        return p1;
    }
    if (m_heapType == MINHEAP){
        //smaller values are higher priorities
        if (m_priorFunc(p2->getPost()) < m_priorFunc(p1->getPost())){
            //comparing priorities of top nodes
            Node* temp = p1;
            p1 = p2;
            p2 = temp;
            temp = nullptr;
        }
    }
    else{ //if this is a maxheap
        if (m_priorFunc(p1->getPost()) < m_priorFunc(p2->getPost())){
            //comparing priorities of top nodes
            Node* temp = p1;
            p1 = p2;
            p2 = temp;
            temp = nullptr;
        }
    }
    Node* temp = p1->m_left;
    p1->m_left = p1->m_right;
    p1->m_right = temp;
    temp = nullptr;
    //p1->m_left = r_merge(p2, p1->m_left);
    p1->m_left = r_merge(p1->m_left, p2);
    p2 = nullptr;
    return p1;
}

void SQueue::r_print(const Node* temp) const{
    //done
    if (temp == nullptr){
        return;
    }
    const Post& post = temp->m_post;
    cout << "[" << m_priorFunc(post) << "]" << " Post ID: "
    << post.getPostID() << ", number of likes: " <<
    post.getNumLikes() << ", interest level: " << post.getInterestLevel() <<
    ", connection level: " << post.getConnectLevel() << ", post time: " <<
    post.getPostTime() << endl;
    r_print(temp->m_left);
    r_print(temp->m_right);
}

void SQueue::r_rebuild(Node* temp){
    //done
    if (temp == nullptr){
        return;
    }
    Node* pright = temp->m_right;
    Node* pleft = temp->m_left;
    temp->m_left = nullptr;
    temp->m_right = nullptr;
    m_heap = r_merge(m_heap, temp);
    r_rebuild(pleft);
    r_rebuild(pright);
    temp = nullptr;
    pright = nullptr;
    pleft = nullptr;
}

bool SQueue::r_heap_property(const Node* temp) const{
    if (temp == nullptr){
        return true; //empty heap is fine
    }
    if (temp->m_left != nullptr){ //has left child
        if (m_heapType == MINHEAP){
            if (m_priorFunc(temp->getPost()) > m_priorFunc(temp->m_left->getPost())){
                return false;
            }
        }
        else{
            if (m_priorFunc(temp->getPost()) < m_priorFunc(temp->m_left->getPost())){
                return false;
            }
        }
    }
    if (temp->m_right != nullptr){ //has right child
        if (m_heapType == MINHEAP){
            if (m_priorFunc(temp->getPost()) > m_priorFunc(temp->m_right->getPost())){
                return false;
            }
        }
        else{ //maxheap
            if (m_priorFunc(temp->getPost()) < m_priorFunc(temp->m_right->getPost())){
                return false;
            }
        }
    }
    return r_heap_property(temp->m_left) && r_heap_property(temp->m_right);
}
