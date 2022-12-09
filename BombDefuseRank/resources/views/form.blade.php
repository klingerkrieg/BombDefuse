@extends('layouts.app')

@section('content')
<div class="container">
    <div class="row justify-content-center">
        <div class="col-md-8">
            <div class="card">
                <div class="card-header">{{ __('Cadastro de equipes') }}</div>

                <div class="card-body">

                    @if ($team->id)
                        <form id="form" action="{{route('update',$team)}}" method="post">
                            @csrf
                    @else
                        <form id="form" action="{{route('save')}}" method="post">
                            @csrf
                    @endif

                        <div class='row'>
                            <label class='col-md'>Nome da equipe
                                <input name="team_name" class="form-control" type="text" value="{{$team->team_name}}">
                            </label>

                            <label class='col-md-4'>Tempo restante
                                <input name="time" class="form-control" type="time" max="18:00" min="00:00" value="{{$team->time}}">
                            </label>
                        </div>

                        @for ($i = 0; $i < 5; $i++)
                        <div>
                            <h5 class="border-bottom mt-5">Membro {{$i+1}}</h5>
                            <div class='row'>

                                <label class='col-md-6'> Nome
                                    <input name="name[{{$i}}]" class="form-control" type="text" value="{{$members[$i]->name ?? null}}">
                                </label>

                                <label class='col-md-2'> Idade
                                    <input name="age[{{$i}}]" class="form-control" type="text" value="{{$members[$i]->age ?? null}}">
                                </label>

                                <label class='col-md-4'> Curso
                                    <input name="course[{{$i}}]" class="form-control" type="text" value="{{$members[$i]->course ?? null}}">
                                </label>

                                {{--<label class='col-md-2'> Período
                                    <input name="course_period[{{$i}}]" class="form-control" type="text" value="">
                                </label>

                                <label class='col-md-3'> Nota para o projeto
                                    <input name="grade[{{$i}}]" class="form-control" type="text" value="">
                                </label>--}}
                            </div>
                        </div>
                        @endfor

                    </form>

                    <div class="row gap-3 mt-3 d-flex justify-content-center">
                        <button type="submit" class="btn btn-primary col-md-2" form="form">Salvar</button>
                        <a href="{{route("new")}}" class="btn btn-secondary col-md-2">Novo</a>
                        <a href="{{route("home")}}" class="btn btn-secondary col-md-2">Rank</a>
                        <a href="{{route("foto",$team)}}" class="btn btn-success col-md-2">Foto</a>
                        @if ($canDelete)
                            <a href="{{route("delete",$team)}}" class="btn btn-danger col-md-2">Deletar</a>                            
                        @endif
                    </div>


                </div>
            </div>
        </div>
    </div>
</div>
@endsection
