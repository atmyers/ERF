#ifndef ERF_BOUNDARYPLANE_H
#define ERF_BOUNDARYPLANE_H

#include "AMReX_Gpu.H"
#include "AMReX_AmrCore.H"
#include <AMReX_BndryRegister.H>
#include "IndexDefines.H"
#include "DataStruct.H"

using PlaneVector = amrex::Vector<amrex::FArrayBox>;

/** Collection of data structures and operations for reading data
 *
 *  This class contains the inlet data structures and operations to
 *  read and interpolate inflow data.
 */
class ReadBndryPlanes
{

public:
    explicit ReadBndryPlanes(const amrex::Geometry& geom,
                             const amrex::Real& rdOcp_in);

    void define_level_data(int lev);

    void read_time_file();

    void read_input_files(amrex::Real time, amrex::Real dt,
        amrex::Array<amrex::Array<amrex::Real, AMREX_SPACEDIM*2>,AMREX_SPACEDIM+NVAR> m_bc_extdir_vals);

    void read_file(int idx, amrex::Vector<std::unique_ptr<PlaneVector>>& data_to_fill,
        amrex::Array<amrex::Array<amrex::Real, AMREX_SPACEDIM*2>,AMREX_SPACEDIM+NVAR> m_bc_extdir_vals);

    // Return the pointer to PlaneVectors at time "time"
    amrex::Vector<std::unique_ptr<PlaneVector>>& interp_in_time(const amrex::Real& time);

    [[nodiscard]] amrex::Real tinterp() const { return m_tinterp; }

    [[nodiscard]] int ingested_velocity() const {return is_velocity_read;}
    [[nodiscard]] int ingested_theta()    const {return (is_temperature_read || is_theta_read);}
    [[nodiscard]] int ingested_density()  const {return is_density_read;}
    [[nodiscard]] int ingested_scalar()   const {return is_scalar_read;}
#if defined(ERF_USE_MOISTURE)
    [[nodiscard]] int ingested_qt()       const {return is_qt_read;}
    [[nodiscard]] int ingested_qp()       const {return is_qp_read;}
#elif defined(ERF_USE_WARM_NO_PRECIP)
    int ingested_qv()       const {return is_qv_read;}
    int ingested_qc()       const {return is_qc_read;}
#endif
    [[nodiscard]] int ingested_KE()       const {return is_KE_read;}
    [[nodiscard]] int ingested_QKE()      const {return is_QKE_read;}

private:

    //! The times for which we currently have data
    amrex::Real m_tn;
    amrex::Real m_tnp1;
    amrex::Real m_tnp2;

    //! Data at time m_tn
    amrex::Vector<std::unique_ptr<PlaneVector>> m_data_n;

    //! Data at time m_tnp1
    amrex::Vector<std::unique_ptr<PlaneVector>> m_data_np1;

    //! Data at time m_tnp2
    amrex::Vector<std::unique_ptr<PlaneVector>> m_data_np2;

    //! Data interpolated to the time requested
    amrex::Vector<std::unique_ptr<PlaneVector>> m_data_interp;

    //! Time for plane at interpolation
    amrex::Real m_tinterp{-1.0};

    //! Geometry at level 0
    amrex::Geometry m_geom;

    //! File name for IO
    std::string m_filename{""};

    //! File name for file holding timesteps and times
    std::string m_time_file{""};

    //! The timesteps / times that we read from time.dat
    amrex::Vector<amrex::Real> m_in_times;
    amrex::Vector<int> m_in_timesteps;

    //! Variables to be read in
    amrex::Vector<std::string> m_var_names;

    //! controls extents on native bndry output
    const int m_in_rad = 1;
    const int m_out_rad = 1;
    const int m_extent_rad = 0;

    //! R_d/c_p is needed for reading boundary files
    const amrex::Real m_rdOcp;

    int is_velocity_read;
    int is_density_read;
    int is_temperature_read;
    int is_theta_read;
    int is_scalar_read;
#if defined(ERF_USE_MOISTURE)
    int is_qt_read;
    int is_qp_read;
#elif defined(ERF_USE_WARM_NO_PRECIP)
    int is_qv_read;
    int is_qc_read;
#endif
    int is_KE_read;
    int is_QKE_read;

    int last_file_read;
};

#endif /* ERF_BOUNDARYPLANE_H */
