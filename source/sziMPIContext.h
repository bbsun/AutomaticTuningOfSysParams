#ifndef _sziMPIContext_h_
#define _sziMPIContext_h_

#include <mpi.h>

#include "sziStreamable.h"
#include <string>

namespace szi
{

    struct MPIContext
    {
        typedef int RankType;
        typedef int NumberOfWorkersType;

        enum
        {
            TAG_EXIT = 0,
            TAG_OK,
            TAG_FAIL,
            TAG_USER = 1000
        };

        static NumberOfWorkersType getNumberOfWorkers()
        {
            int nranks;
            MPI_Comm_size( MPI_COMM_WORLD, &nranks );
            return nranks;
        }

        static RankType getWorkerRank()
        {
            RankType rank;
            MPI_Comm_rank( MPI_COMM_WORLD, &rank );
            return rank;
        }

        static void terminateAllSlaves()
        {
            NumberOfWorkersType nranks = getNumberOfWorkers();
            //
            for ( RankType i = 1; i < (RankType)nranks; i++ )
            {
                MPIContext::send( i, TAG_EXIT );
            }
        }

        /** Send a single single tag to a destination. */
        static void send( RankType rank, int tag )
        {
            MPI_Send
            (
                0, // message buffer
                0, // how many data item in the buffer
                MPI_CHAR, // data type of each item
                rank, // destination process
                tag, // user chosen message tag
                MPI_COMM_WORLD // communicator to send the message
            );
        }

        /** Receive a single tag from a source. */
        static int receive( RankType rank )
        {
            MPI_Status status;
            MPI_Recv
            (
                0, // message buffer
                0, // how many data item
                MPI_CHAR, // data type of each item
                rank, // source process
                MPI_ANY_TAG, // user-defined message tag
                MPI_COMM_WORLD, // communicator to receive the message
                &status // info about the received message
            );
            return status.MPI_TAG;
        }

        // sending/receiving data of type szi::Streamable through MPI
        static void send( const Streamable& s, RankType rank, int tag )
        {
            StreamBuffer sb;
            sb << s;
            //
            long size = sb.getSize();
            MPI_Send
            (
                &size,
                1,
                MPI_LONG,
                rank,
                tag,
                MPI_COMM_WORLD
            );
            //
            MPI_Send
            (
                sb.getPointer(),
                size,
                MPI_CHAR,
                rank,
                tag,
                MPI_COMM_WORLD
            );
        }
        //
        static void receive( Streamable& s, RankType rank, int tag )
        {
            MPI_Status status;
            //
            long size = 0;
            MPI_Recv
            (
                &size,
                1,
                MPI_LONG,
                rank,
                tag,
                MPI_COMM_WORLD,
                &status
            );
            //
            char* buf = new char[size];
            MPI_Recv
            (
                buf,
                size,
                MPI_CHAR,
                rank,
                tag,
                MPI_COMM_WORLD,
                &status
            );
            //
            StreamBuffer sb;
            sb.streamIn( buf, size );
            delete[] buf;
            sb >> s;
        }

        // sending/receiving data of type std::string through MPI
        static void send( const std::string& s, RankType rank, int tag )
        {
            int size = (int)s.size();
            MPI_Send
            (
                &size,
                1,
                MPI_INT,
                rank,
                tag,
                MPI_COMM_WORLD
            );
            //
            MPI_Send
            (
                (void*)s.data(),
                size,
                MPI_CHAR,
                rank,
                tag,
                MPI_COMM_WORLD
            );
        }
        //
        static void receive( std::string& s, RankType rank, int tag )
        {
            MPI_Status status;
            //
            int size = 0;
            MPI_Recv
            (
                &size,
                1,
                MPI_INT,
                rank,
                tag,
                MPI_COMM_WORLD,
                &status
            );
            //
            char* buf = new char[size];
            MPI_Recv
            (
                buf,
                size,
                MPI_CHAR,
                rank,
                tag,
                MPI_COMM_WORLD,
                &status
            );
            //
            s = std::string( buf, size );
            delete[] buf;
        }

        // sending/receiving data of basic types through MPI
        template < typename T >
        static void send( const T& data, RankType rank, int tag )
        {
            MPI_Send
            (
                (void*)&data,
                sizeof(T),
                MPI_CHAR,
                rank,
                tag,
                MPI_COMM_WORLD
            );
        }
        //
        template < typename T >
        static void receive( T& data, RankType rank, int tag )
        {
            MPI_Status status;
            //
            MPI_Recv
            (
                &data,
                sizeof(T),
                MPI_CHAR,
                rank,
                tag,
                MPI_COMM_WORLD,
                &status
            );
        }
    };

} // namespace szi

#endif // _sziMPIContext_h_
