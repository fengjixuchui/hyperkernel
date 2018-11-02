//
// Bareflank Hyperkernel
// Copyright (C) 2018 Assured Information Security, Inc.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

#ifndef EVTCHN_INTEL_X64_HYPERKERNEL_H
#define EVTCHN_INTEL_X64_HYPERKERNEL_H

#include "../base.h"
#include "public/event_channel.h"

// -----------------------------------------------------------------------------
// Exports
// -----------------------------------------------------------------------------

#include <bfexports.h>

#ifndef STATIC_HYPERKERNEL_HVE
#ifdef SHARED_HYPERKERNEL_HVE
#define EXPORT_HYPERKERNEL_HVE EXPORT_SYM
#else
#define EXPORT_HYPERKERNEL_HVE IMPORT_SYM
#endif
#else
#define EXPORT_HYPERKERNEL_HVE
#endif

// -----------------------------------------------------------------------------
// Definitions
// -----------------------------------------------------------------------------

namespace hyperkernel::intel_x64
{

class EXPORT_HYPERKERNEL_HVE evtchn
{
public:

    enum state : uint8_t {
        free,
        reserved,
        unbound,
        interdomain,
        pirq,
        virq,
        ipi
    };

    /// Constructor
    ///
    /// @expects
    /// @ensures
    ///
    /// @param vcpu the vcpu of the evtchn_fifo
    ///
    evtchn() = default;

    /// Destructor
    ///
    /// @expects
    /// @ensures
    ///
    ~evtchn() = default;

    inline auto is_pending() const
    { return m_is_pending; }

    inline auto state() const
    { return m_state; }

    inline auto priority() const
    { return m_priority; }

    inline auto prev_priority() const
    { return m_prev_priority; }

    inline auto vcpuid() const
    { return m_vcpuid; }

    inline auto prev_vcpuid() const
    { return m_prev_vcpuid; }

    inline auto port() const
    { return m_port; }

    inline auto set_pending(bool pending)
    { m_is_pending = pending; }

    inline auto set_state(enum state state)
    { m_state = state; }

    inline auto set_priority(uint8_t priority)
    { m_priority = priority; }

    inline auto set_prev_priority(uint8_t prev_priority)
    { m_prev_priority = prev_priority; }

    inline auto set_vcpuid(vcpuid_t id)
    { m_vcpuid = id; }

    inline auto set_prev_vcpuid(vcpuid_t id)
    { m_prev_vcpuid = id; }

    inline auto set_port(evtchn_port_t port)
    { return m_port = port; }

private:

    bool m_is_pending{};
    enum state m_state{free};

    uint8_t m_priority{EVTCHN_FIFO_PRIORITY_DEFAULT};
    uint8_t m_prev_priority{EVTCHN_FIFO_PRIORITY_DEFAULT};

    vcpuid_t m_vcpuid{};
    vcpuid_t m_prev_vcpuid{};

    evtchn_port_t m_port{};
    mutable std::mutex m_mutex{};

public:

    /// @cond

    evtchn(evtchn &&) = default;
    evtchn &operator=(evtchn &&) = default;

    evtchn(const evtchn &) = delete;
    evtchn &operator=(const evtchn &) = delete;

    /// @endcond
};

constexpr auto is_power_of_two(size_t val)
{ return (val > 0) && ((val & (val - 1)) == 0); }

constexpr auto next_power_of_two(size_t val)
{
    while (!is_power_of_two(val)) {
        val++;
    }
    return val;
}

constexpr auto evtchn_size = next_power_of_two(sizeof(class evtchn));
constexpr auto evtchn_per_page = ::x64::pt::page_size / evtchn_size;

}

#endif