#ifndef SIMPLEX_BOUNDARY_H
#define SIMPLEX_BOUNDARY_H
#include "term/term.h"
#include "finite_field/finite_field.h"

//non-exported functionality
namespace {

template< typename Term_>
class const_boundary_iterator : 
	public std::iterator< std::bidirectional_iterator_tag,
			      Term_,
			      std::ptrdiff_t,
			      const Term_*,
			      const Term_>{
	private:
	  typedef const_boundary_iterator< Term_> Self;
	  typedef Term_ Term;
	  typedef typename Term::Cell Cell;
	  typedef typename Cell::value_type Vertex;
	public:
	  //default constructor
	const_boundary_iterator(): cellptr( 0), pos( 0){}
	const_boundary_iterator( const Cell & s): pos( 0){ 
		if( s.dimension()){
		    //begin by removing first vertex
		    cellptr = &s;
		    removed = s.vertices[ 0];
		    face.coefficient( 1);
		    face.cell().vertices.resize( s.dimension());
		    std::copy( s.begin()+1, s.end(), 
			       face.cell().vertices.begin()); 
		}else{ cellptr = 0; } //\partial(vertex) = 0
	}
	//copy constructor
	const_boundary_iterator( const Self & from): cellptr( from.cellptr), 
	face(from.face), pos( from.pos), removed( from.removed){}
	const_boundary_iterator& operator==( const Self & b) const { 
		return (b.cellptr == cellptr) && (b.pos == pos); 
	} 
	const_boundary_iterator& operator=(const Self & b){
		cellptr = b.cellptr;
		remove = b.removed;
		face = b.face;
		pos = b.pos;
		return *this;
	}
	Term& operator*() { 
		if( cellptr != 0) { return face; } 
		std::cerr << "Cannot Dereference End" << std::endl; 
		std::exit( -1); 
	}
	const Term* operator->() const { return &face; }
	bool operator!=( const const_boundary_iterator & b) const { 
		return (b.cellptr != cellptr) || (b.pos != pos); 
	}

	const_boundary_iterator& operator++(){
		if( pos == face.cell().size()){
			cellptr = 0;
			pos = 0;
			return *this;
		}
		//return removed vertex, get rid of another one
		std::swap( face.cell().vertices[ pos++], removed);
		face.coefficient( -1*face.coefficient());
		return *this;	
	}

	const_boundary_iterator& operator--(){
		if(pos == 0){
			cellptr = 0;
			pos = 0;
			return *this;
		}
		//return removed vertex, get rid of another one
		std::swap( face.cell().vertices[ --pos], removed);
		face.coefficient( -1*face.coefficient());
		return *this;	
	}

	const_boundary_iterator operator++( int){
	 	const_boundary_iterator tmp( *this); 
		++(*this); //now call previous operator
		return tmp;
	}
	const_boundary_iterator operator--( int){
	 	const_boundary_iterator tmp( *this); 
		--(*this); //now call previous operator
		return tmp;
	}
	 private:
		const Cell* cellptr;
		typename Cell::size_t pos;
		Term face;
		Vertex removed;
}; // END const_boundary_iterator
} //END private namespace

namespace ctl { 
template< typename Simplex_, typename Coefficient_>
class Simplex_boundary {
public:
	typedef Simplex_ Simplex;
	typedef Coefficient_ Coefficient;
	typedef ctl::Term< Simplex, Coefficient> Term;
	typedef const_boundary_iterator< Term> const_iterator;
	//default constructor
	Simplex_boundary(){};	

	Simplex_boundary& operator=( const Simplex_boundary& from){ 
		return *this;
	}
	//It only makes sense for const iterators
	const_iterator begin( const Simplex & s) { return const_iterator( s); }
	const_iterator end( const Simplex & s)   { return const_iterator(); }
	std::size_t length( const Simplex & s) const { 
		return (s.size()>1)? s.size():0;
	}
}; //Simplex_boundary

} // namespace ctl

#endif //SIMPLEX_BOUNDARY_H
