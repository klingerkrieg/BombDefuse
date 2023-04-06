@extends('layouts.app')

@section('content')

<style>

#logo{
    width:150px;
}


</style>
<script>
function verEquipe(id){
    document.getElementById(id).className = "";
}
</script>
<div class="container">
    <div class="row justify-content-center">
        <div class="col-md-8">
            <div class="card">
                
                <div class="card-body d-flex align-items-center flex-column">
                    
                    
                    <img id="logo" class="" src="{{asset('bomb.png')}}">

                    <table class="table">
                    <thead>
                        <tr>
                        <th scope="col">#</th>
                        <th scope="col">Tempo</th>
                        <th scope="col">Equipe</th>
                        <th scope="col">Pontuação</th>
                        <th></th>
                        </tr>
                    </thead>
                    <tbody>
                        @foreach ($teams as $k=>$team)
                            <tr>
                                <th scope="row">{{$k+1}}</th>
                                <td>{{$team['time']}}</td>
                                <td>{{$team['team_name']}}
                                    
                                    
                                    @if ($team['exploded'] == 1)
                                        <img src="{{asset("explode.png")}}" style="height:22px;">
                                    @elseif ($k == 0)
                                        <img src="{{asset("trofeu.png")}}" style="height:22px;">
                                    @elseif ($k == 1)
                                        <img src="{{asset("trofeu2.png")}}" style="height:22px;">
                                    @elseif ($k == 2)
                                        <img src="{{asset("trofeu3.png")}}" style="height:22px;">
                                    @else
                                        <img src="{{asset("alicate.png")}}" style="height:22px;">
                                    @endif
                                </td>
                                <td>
                                    {{number_format($team->score,2)}}
                                </td>
                                <td><a href='#' onclick="verEquipe({{$team['id']}})">Ver equipe</a></td>
                            </tr>

                            <tr id='{{$team['id']}}' class='visually-hidden'>
                                <td colspan="4">
                                    <ul>
                                        @foreach ($team['members'] as $member)
                                            <li>{{$member['name']}} ({{$member['course']}})</li>
                                        @endforeach
                                    </ul>
                                </td>
                            </tr>
                            
                        @endforeach
                    </tbody>
                    </table>

                    

                </div>
            </div>
        </div>
    </div>
</div>
@endsection

