from django.shortcuts import render
from django.http import HttpResponse, Http404
from django.template import loader
from django.db.models import Q

from .models import Player, Team, State, Color

# Create your views here.
def index(request):

    s = '<h1>Hello, world! You are at the demo index page.<br></h1>' \
        'Some possible queries below:<br><br>' \
        '<a href="/demo/query1/1/35/40/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/">query1 example</a><br>' \
        '<a href="/demo/query2/Orange/">query2 example</a><br>' \
        '<a href="/demo/query3/Duke/">query3 example</a><br>' \
        '<a href="/demo/query4/NC/LightBlue/">query4 example</a><br>' \
        '<a href="/demo/query5/13/">query5 example</a>'

    return HttpResponse(s)


def query1(request, use_mpg, min_mpg, max_mpg, use_ppg, min_ppg, max_ppg, use_rpg, min_rpg, max_rpg, use_apg, min_apg, max_apg, use_spg, min_spg, max_spg, use_bpg, min_bpg, max_bpg):
    try:
        q = ~Q(pk=None)  # an always true q object, like AND TRUE;
        if (use_mpg != 0):
            q11 = Q(mpg__lte=max_mpg)
            q12 = Q(mpg__gte=min_mpg)
            q = q & q11 & q12
        if (use_ppg != 0):
            q21 = Q(ppg__lte=max_ppg)
            q22 = Q(ppg__gte=min_ppg)
            q = q & q21 & q22
        if (use_rpg != 0):
            q31 = Q(rpg__lte=max_rpg)
            q32 = Q(rpg__gte=min_rpg)
            q = q & q31 & q42
        if (use_apg != 0):
            q41 = Q(apg__lte=max_apg)
            q42 = Q(apg__gte=min_apg)
            q = q & q41 & q42
        if (use_spg != 0):
            q51 = Q(spg_lte=float(max_spg))
            q52 = Q(spg__gte=float(min_spg))
            q = q & q51 & q52
        if (use_bpg != 0):
            q61 = Q(bpg__lte=float(max_bpg))
            q62 = Q(bpg__gte=float(min_bpg))
            q = q & q61 & q62

        result_list = Player.objects.filter(q)
    except Player.DoesNotExist:
        raise Http404("query1 player results does not exist")
    except:
        raise Http404("query1 query failed. check your input")
    result_list = [[res.player_id, res.team_id, res.uniform_num, res.first_name, res.last_name, res.mpg, res.ppg, res.rpg, res.apg, res.spg, res.bpg] for res in result_list]
    context = {
        'result_list': result_list,
    }
    return render(request, 'demo/query1.html', context)
    

def query2(request, team_color):
    try:
        color_list = Color.objects.get(name=team_color)
        result_list = Team.objects.filter(color__in=[color_list.color_id])
    except Color.DoesNotExist:
        raise Http404("query2 color item does not exist")
    except Team.DoesNotExist:
        raise Http404("query2 team results does not exist")
    except:
        raise Http404("query2 query failed. check your input")
    result_list = [[res.name] for res in result_list]
    context = {
        'result_list': result_list,
    }
    return render(request, 'demo/query2.html', context)


def query3(request, team_name):
    try:
        team_list = Team.objects.get(name=team_name)
        result_list = Player.objects.filter(team__in=[team_list.team_id]).order_by('-ppg')
    except Team.DoesNotExist:
        raise Http404("query3 team item does not exist")
    except Player.DoesNotExist:
        raise Http404("query3 player results does not exist")
    except:
        raise Http404("query3 query failed. check your input")
    result_list = [[res.first_name, res.last_name] for res in result_list]
    context = {
        'result_list': result_list,
    }
    return render(request, 'demo/query3.html', context)


def query4(request, team_state, team_color):
    try:
        state_list = State.objects.get(name=team_state)
        color_list = Color.objects.get(name=team_color)
        team_list = Team.objects.get(Q(color__in=[color_list.color_id]) & Q(state__in=[state_list.state_id]))

        result_list = Player.objects.filter(team__in=[team_list.team_id])
    except Player.DoesNotExist:
        raise Http404("query4 player results does not exist")
    except:
        raise Http404("query4 query failed. check your input")
    result_list = [[res.first_name, res.last_name, res.uniform_num] for res in result_list]
    context = {
        'result_list': result_list,
    }
    return render(request, 'demo/query4.html', context)


def query5(request, num_wins):
    try:
        team_list = Team.objects.filter(wins__gt=num_wins)
        temp_team_dict = dict()
        player_list = []
        for team in team_list:
            temp_team_dict[team.team_id] = (team.name, team.wins)
            player_list.extend(Player.objects.filter(team__in=[team.team_id]))
        
    except Player.DoesNotExist:
        raise Http404("query5 player results does not exist")
    except:
        raise Http404("query5 query failed. check your input")

    result_list = [[res.first_name, res.last_name, temp_team_dict[res.team.team_id][0], temp_team_dict[res.team.team_id][1]] for res in player_list]
    context = {
        'result_list': result_list,
    }
    return render(request, 'demo/query5.html', context)

