from skyfield.api import load
import sys

from skyfield.api import load, wgs84
from datetime import datetime
from zoneinfo import ZoneInfo

import numpy as np
np.set_printoptions(formatter={'all': '{:0.3e}'.format})

eph =     load('de421.bsp')

sun =   eph['sun']
earth = eph['earth']
moon =  eph['moon']

ts = load.timescale()
t = ts.now()
#print("now:", t.utc_iso())

#   sun_relative_to_ssb = planets['sun'].at(time_obj)

s = sun.at(t)
e = earth.at(t)
m = moon.at(t)

s_vel = s.velocity.m_per_s
e_vel = e.velocity.m_per_s
m_vel = m.velocity.m_per_s

sun_mass        = 1.989e30
earth_mass      = 5.972e24
moon_mass       = 7.346e22

sun_mass_norm   = sun_mass / sun_mass
earth_mass_norm = earth_mass / sun_mass
moon_mass_norm  = moon_mass / sun_mass

print(f"Body sun_mks =        {{{s.position.m[0]:+0.3e}, {s.position.m[1]:+0.3e}, {s.position.m[2]:+0.3e}, {s_vel[0]:+0.3e}, {s_vel[1]:+0.3e}, {s_vel[2]:+0.3e}, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.989e30}};")
print(f"Body earth_mks =      {{{e.position.m[0]:+0.3e}, {e.position.m[1]:+0.3e}, {e.position.m[2]:+0.3e}, {e_vel[0]:+0.3e}, {e_vel[1]:+0.3e}, {e_vel[2]:+0.3e}, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 5.972e24}};")
print(f"Body moon_mks  =      {{{m.position.m[0]:+0.3e}, {m.position.m[1]:+0.3e}, {m.position.m[2]:+0.3e}, {m_vel[0]:+0.3e}, {m_vel[1]:+0.3e}, {m_vel[2]:+0.3e}, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 7.346e22}};")

print()

one   = 1.e0
one_e3 = 1.e3

Astronomical_unit_mks = 1.496e11
au_mks= Astronomical_unit_mks

m_to_au_m = one/au_mks

Seconds_in_Day = 8.64e4
sd = Seconds_in_Day

Astronomical_unit_norm = 1.496e8   # km/AU
aumn = Astronomical_unit_norm*one_e3;

m_per_s_to_au_per_day = one*sd/aumn;
#print(f"m_per_s_to_au_per_day = {m_per_s_to_au_per_day:+0.3e}")

print(f"Body sun_norm =       {{{s.position.m[0]*m_to_au_m:+0.3e}, {s.position.m[1]*m_to_au_m:+0.3e}, {s.position.m[2]*m_to_au_m:+0.3e}, {s_vel[0]*m_per_s_to_au_per_day:+0.3e}, {s_vel[1]*m_per_s_to_au_per_day:+0.3e}, {s_vel[2]*m_per_s_to_au_per_day:+0.3e}, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, {sun_mass_norm:+0.3e}}};")
print(f"Body earth_norm =     {{{e.position.m[0]*m_to_au_m:+0.3e}, {e.position.m[1]*m_to_au_m:+0.3e}, {e.position.m[2]*m_to_au_m:+0.3e}, {e_vel[0]*m_per_s_to_au_per_day:+0.3e}, {e_vel[1]*m_per_s_to_au_per_day:+0.3e}, {e_vel[2]*m_per_s_to_au_per_day:+0.3e}, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, {earth_mass_norm:+0.3e}}};")
print(f"Body moon_norm =      {{{m.position.m[0]*m_to_au_m:+0.3e}, {m.position.m[1]*m_to_au_m:+0.3e}, {m.position.m[2]*m_to_au_m:+0.3e}, {m_vel[0]*m_per_s_to_au_per_day:+0.3e}, {m_vel[1]*m_per_s_to_au_per_day:+0.3e}, {m_vel[2]*m_per_s_to_au_per_day:+0.3e}, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, {moon_mass_norm:+0.3e}}};")

print()

print(f"Body sun =            {{{s.position.m[0]*m_to_au_m:+0.3e}, {s.position.m[1]*m_to_au_m:+0.3e}, {s.position.m[2]*m_to_au_m:+0.3e}, {s_vel[0]*m_per_s_to_au_per_day:+0.3e}, {s_vel[1]*m_per_s_to_au_per_day:+0.3e}, {s_vel[2]*m_per_s_to_au_per_day:+0.3e}, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, {sun_mass_norm:+0.3e}}};")
print(f"Body earth =          {{{e.position.m[0]*m_to_au_m:+0.3e}, {e.position.m[1]*m_to_au_m:+0.3e}, {e.position.m[2]*m_to_au_m:+0.3e}, {e_vel[0]*m_per_s_to_au_per_day:+0.3e}, {e_vel[1]*m_per_s_to_au_per_day:+0.3e}, {e_vel[2]*m_per_s_to_au_per_day:+0.3e}, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, {earth_mass_norm:+0.3e}}};")
print(f"Body moon =           {{{m.position.m[0]*m_to_au_m:+0.3e}, {m.position.m[1]*m_to_au_m:+0.3e}, {m.position.m[2]*m_to_au_m:+0.3e}, {m_vel[0]*m_per_s_to_au_per_day:+0.3e}, {m_vel[1]*m_per_s_to_au_per_day:+0.3e}, {m_vel[2]*m_per_s_to_au_per_day:+0.3e}, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, {moon_mass_norm:+0.3e}}};")
