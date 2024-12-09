@extends('layouts.app')

@section('title', 'Rank - Defuse The Bomb')

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
<main>

    <div class="container my-4">

        <img class="logo" src="{{asset('img/logo.png')}}">
        <div class="d-flex flex-column align-items-center mb-4">
            <h2 class="text-center fw-bold">Rank</h2>
            <a class="btn btn-dark" href="{{ route('home') }}">
                <i class="bi bi-arrow-left"></i>
                Voltar
            </a>
        </div>
        
        <div class="table-responsive">
            <table class="table table-hover">
                <thead>
                    <tr>
                        <th scope="col">#</th>
                        <th scope="col">Tempo</th>
                        <th scope="col">Equipe</th>
                        <th scope="col">Pontuação</th>
                        @if(Auth::check())
                            <th scope="col">Ações</th>
                        @endif
                    </tr>
                </thead>
                <tbody>
                    @php 
                    $position = 0;
                    @endphp
                    @foreach ($teams as $k=>$team)
                        @php 
                        $position++;
                        @endphp
                        <tr class="{{ $position <= 3 ? 'table-success' : ''}}">
                            <th scope="row">{{$position}}</th>
                            <td>{{$team['time']}}</td>
                            <td>{{$team['team_name']}}

                                @if ($team['exploded'] == 1)
                                    <img src="{{asset("img/explode.png")}}" style="height:22px;">
                                @elseif ($position == 1)
                                    <img src="{{asset("img/trofeu.png")}}" style="height:22px;">
                                @elseif ($position == 2)
                                    <img src="{{asset("img/trofeu2.png")}}" style="height:22px;">
                                @elseif ($position == 3)
                                    <img src="{{asset("img/trofeu3.png")}}" style="height:22px;">
                                @else
                                    <img src="{{asset("img/alicate.png")}}" style="height:22px;">
                                @endif

                            </td>
                            <td>
                                {{number_format($team->score,2)}}
                            </td>
                            
                            @if(Auth::check())
                                <td><a href='#' onclick="verEquipe({{$team['id']}})">Ver equipe</a></td>
                            @endif
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
   
</main>

<script>

    // Atualizando Rank
    setInterval(() => {
        window.location.reload();
    }, 30000)
    
</script>

@endsection

