#ifndef TREEVIEWSTATE_H_
#define TREEVIEWSTATE_H_

#include <QTreeView>
namespace daq
{
namespace QTUtils
{

/**
 * Stores the expanded and selected state of items in a QTreeView.
 * The class can be used with any TreeView.
 * Usage:
 * \code 
 *  TreeViewState state = TreeViewState::storeState(myview);
 *  ...
 *  TreeViewState::restoreState(myview,state);
 *  \endcode
 */
class TreeViewState
{
 public:
    static TreeViewState storeState(QTreeView* view);
    static void resetState(QTreeView* view, TreeViewState& state);
	
    TreeViewState(){};
    TreeViewState& operator=(const TreeViewState &other)
    {
        expandedChildren = other.expandedChildren;
        selectedChildren = other.selectedChildren;
        return *this;
    }
    TreeViewState(const TreeViewState& other)
    {
        this->expandedChildren = other.expandedChildren;
        this->selectedChildren = other.selectedChildren;

    }
    private:
    QHash<QString,TreeViewState> expandedChildren;
    QHash<QString,TreeViewState> selectedChildren;
    void saveIndex(QTreeView* view,QModelIndex parent);
    void resetSubTree(QTreeView* view, QModelIndex ind);
};

/**
 * saves the expanded and selected state of an index
 */
inline void TreeViewState::saveIndex(QTreeView* view,QModelIndex parent)
{
    for(int i =0 ; i< view->model()->rowCount(parent); ++i )
    {
        QModelIndex p = view->model()->index(i,0,parent);
        if(view->isExpanded(p))
        {
            TreeViewState child;
            ERS_DEBUG(0,"This is expanded : " << view->model()->data(p).toString().toStdString());
            child.saveIndex(view,p);
            this->expandedChildren.insert(view->model()->data(p).toString(),child);

        }
        if(view->selectionModel()->isSelected(p))
        {
            TreeViewState child;
            ERS_DEBUG(0,"This is selected : " << view->model()->data(p).toString().toStdString());
            this->selectedChildren.insert(view->model()->data(p).toString(),child);
        }

    }
}

/**
 * This method will save the entire layout of the TreeView.
 * To be used to keep state between model rebuilds
 */
inline TreeViewState TreeViewState::storeState(QTreeView* view)
{
    QAbstractItemModel* model = view->model();
    TreeViewState state;
    if(model)
    {
        state.saveIndex(view,view->rootIndex());
    }
    return state;
}

/**
 * recursive method for restoring all expanded items and reselecting selected ones
 */
inline void TreeViewState::resetSubTree(QTreeView* view, QModelIndex ind)
{
    if(ind.isValid())
    {
        view->setExpanded(ind,true);
    }
    for(int i =0 ; i< view->model()->rowCount(ind); ++i )
    {
        QModelIndex p = view->model()->index(i,0,ind);

        if(this->expandedChildren.contains(view->model()->data(p).toString()))
        {
            TreeViewState s = this->expandedChildren.value(view->model()->data(p).toString());
            s.resetSubTree(view,p);
        }
        if(this->selectedChildren.contains(view->model()->data(p).toString()))
        {
            view->selectionModel()->select(p,QItemSelectionModel::Select);
            view->scrollTo(p);
        }
    }

}
/**
 * resets the treeview to the provided state
 */
inline void TreeViewState::resetState(QTreeView* view,TreeViewState& state)
{
    view->clearSelection();
    view->collapseAll();
    if(state.expandedChildren.size() > 0 || state.selectedChildren.size() > 0)
        state.resetSubTree(view,view->rootIndex());

}

}//namespace QtUtils
}//namespace daq

#endif /*TREEVIEWSTATE_H_*/
