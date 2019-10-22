
#pragma once
#include <VARE.h>

VARE_NAMESPACE_BEGIN

template<class TT> 
class SparseMatrix
{
    private:

        TT*     _values=0;
        size_t* _indices=0;

        size_t _rows=0;
        size_t _columns=0;

    public:

        VARE_UNIFIED
        SparseMatrix()
        {}
        VARE_UNIFIED
        ~SparseMatrix()
        {}

        void initialize( const size_t inRow, const size_t inCol )
        {
            if( inRow*inCol != _rows*_columns )
            {
                finalize();

                //_values = new TT[ inRow*inCol ];
                //_indices = new size_t[ inRow*inCol ];
                cudaMallocManaged( &_values , sizeof(TT)*inRow*inCol     );
                cudaMallocManaged( &_indices, sizeof(size_t)*inRow*inCol );
            }

            _rows = inRow;
            _columns = inCol;            
        }

        void finalize()
        {
            if( _values  ) cudaFree( (void*)_values  );
            if( _indices ) cudaFree( (void*)_indices );

            _values  = 0;
            _indices = 0;
            _rows    = 0;
            _columns = 0;
        }

        VARE_UNIFIED size_t rows()    const { return _rows;    }
        VARE_UNIFIED size_t columns() const { return _columns; }

        VARE_UNIFIED 
        TT* valuesOnRow( const size_t r ) const
        {
            return _values+( r * _columns );
        }

        VARE_UNIFIED 
        size_t* indicesOnRow( const size_t r ) const
        {
            return _indices+( r * _columns );
        }

        void print( const char* name ) const
        {
            std::ofstream file;
            file.open( name );

            for( size_t r=0; r<_rows; ++r )
            {
                std::vector<int> row;
                row.resize( _rows );
                std::fill( row.begin(), row.end(), 0 );

                TT* vals = valuesOnRow( r );
                size_t* inds = indicesOnRow( r );

                for( size_t c=0; c<_columns; ++c )
                {
                    if( inds[c] != MAX_SIZE_T ) row[ inds[c] ] = vals[c];
                }

                for( size_t n=0; n<_rows; ++n )
                {
                    file << row[n] << " ";
                }

                file << '\n';
            }

            file.close();
        }
};

typedef SparseMatrix<int>    IntSparseMatrix;
typedef SparseMatrix<long>   LongSparseMatrix;
typedef SparseMatrix<float>  FloatSparseMatrix;
typedef SparseMatrix<double> DoubleSparseMatrix;

VARE_NAMESPACE_END
