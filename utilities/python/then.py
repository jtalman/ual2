from skyfield.api import load
import sys

from skyfield.api import load, wgs84
from datetime import datetime
from zoneinfo import ZoneInfo

import numpy as np
np.set_printoptions(formatter={'all': '{:0.3e}'.format})

def get_sun_earth_moon_pos_vel(year, month, day, hour, minute, second):
    eph =     load('de421.bsp')

    sun =   eph['sun']
    earth = eph['earth']
    moon =  eph['moon']

#   print('Params=', year, month, day, hour, minute, second)

    utc_dt_zoneinfo = datetime(int(year), int(month), int(day), int(hour), int(minute), int(second), tzinfo=ZoneInfo("UTC"))

    ts = load.timescale()
    time_obj = ts.utc(int(year), int(month), int(day), int(hour), int(minute), int(second))

#   sun_relative_to_ssb = planets['sun'].at(time_obj)

    s = sun.at(time_obj)
    e = earth.at(time_obj)
    m = moon.at(time_obj)

    s_vel = s.velocity.m_per_s
    e_vel = e.velocity.m_per_s
    m_vel = m.velocity.m_per_s

    print(f"Body sun_mks =        {{{s.position.m[0]:+0.3e}, {s.position.m[1]:+0.3e}, {s.position.m[2]:+0.3e}, {s_vel[0]:+0.3e}, {s_vel[1]:+0.3e}, {s_vel[2]:+0.3e}, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, msn}};")
    print(f"Body earth_mks =      {{{e.position.m[0]:+0.3e}, {e.position.m[1]:+0.3e}, {e.position.m[2]:+0.3e}, {e_vel[0]:+0.3e}, {e_vel[1]:+0.3e}, {e_vel[2]:+0.3e}, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, men}};")
    print(f"Body moon_mks  =      {{{m.position.m[0]:+0.3e}, {m.position.m[1]:+0.3e}, {m.position.m[2]:+0.3e}, {m_vel[0]:+0.3e}, {m_vel[1]:+0.3e}, {m_vel[2]:+0.3e}, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, mmn}};")

year   = sys.argv[1] 
month  = sys.argv[2] 
day    = sys.argv[3]  
hour   = sys.argv[4]  
minute = sys.argv[5]  
second = sys.argv[6]  
print('Params=', year, month, day, hour, minute, second)
get_sun_earth_moon_pos_vel(year, month, day, hour, minute, second)
